/**
 * gui_server.c
 * 
 * Server for the GUI client to connect to.
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "gui_server.h"
#include "comm_terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int server_init()
{
    // Create pipes, this fails if they exist, no worries
    if (mknod(pc_to_gui_s, S_IFIFO | 0660, 0) < 0)
    {
        perror("mknod pc_to_gui");
    }
    if (mknod(gui_to_pc_s, S_IFIFO | 0660, 0) < 0)
    {
        perror("mknod gui_to_pc");
    }

    // Open pipes
    if ((fd_wr = open(pc_to_gui_s, O_RDWR | O_NONBLOCK)) < 0)
    {
        perror("open pc_to_gui");
    }
    if ((fd_rd = open(gui_to_pc_s, O_RDWR | O_NONBLOCK)) < 0)
    {
        perror("open gui_to_pc");
    }

    // Clear out pipes
    char buffer[2];
    buffer[1] = '\0';
    while (read(fd_rd, buffer, 1) > 0) { }
    while (read(fd_wr, buffer, 1) > 0) { }

    return 0;
}

int server_disconnect()
{
    close(fd_wr);
    close(fd_rd);
    return 0;
}

char server_read()
{
    char buffer[2];
    buffer[1] = '\0';

    if (read(fd_rd, buffer, 1) > 0)
    {
        return buffer[0];
    }
    else
    {
        return -1;
    }
}

int server_write(char* buffer, int size)
{   
    if (write(fd_wr, buffer, size) < 0)
    {
        // perror("write");
        return -1;
    }
    return 0;
}

int server_write_c(char c)
{
    char buffer[2];
    buffer[0] = c;
    buffer[1] = '\0';
    return server_write(buffer, 1);
}

void comm_gui_init()
{
    constr_byte_queue(&gui_rcv_queue);
}

void comm_gui_update_rcv()
{
    // Store incoming byte in receive queue
    char c;
    if ((c = server_read()) != -1) 
    {
        byte_enqueue(&gui_rcv_queue, c);
    }

    // Empty queue looking for frames
    frame_p frame = comm_gui_rcv_frame();
    if (frame != NULL) 
    {
        switch(frame->header.type) 
        {
            case control_mode_payload:
                rcv_control_mode(frame->payload);
                break;
            case parameter_payload:
                rcv_parameters(frame->payload);
                break;
            case key_payload:
            break;
            default:
                // ignore
                printf("%d\n", frame->header.type);
                break;
        }
        destr_frame(frame);
    }
}

void comm_gui_snd_frame(frame_p frame)
{
#ifdef GUI
    for (uint8_t i = 0; i < sizeof(frame_hdr_t); i++) {
        server_write_c(((byte_p) &frame->header)[i]);
    }
    for (uint8_t i = 0; i < frame->header.size; i++) {
        server_write_c(frame->payload[i]);
    }
    for (uint8_t i = 0; i < sizeof(frame_ftr_t); i++) {
        server_write_c(((byte_p) &frame->footer)[i]);
    }
#endif
}

// Incoming frame state
enum {
    read_preamble,
    read_type,
    read_size,
    read_payload,
    read_checksum,
    read_complete
} rcv_gui_state = read_preamble;
frame_t rcv_gui_frame;
uint16_t rcv_gui_preamble_count = 0;
uint16_t rcv_gui_byte_count = 0;

frame_p comm_gui_rcv_frame() 
{
    // Empty queue of incoming bytes
    while (gui_rcv_queue.count > 0) {
        byte_t b = byte_dequeue(&gui_rcv_queue);

        // Check for preamble
        if (b == 'a') {
            rcv_gui_preamble_count += 1;
            if (rcv_gui_preamble_count == preamble_len) {
                // We read a preamble, restart read
                rcv_gui_state = read_preamble + 1;
                rcv_gui_preamble_count = 0;
                rcv_gui_byte_count = 0;
                continue;
            }
        } else {
            rcv_gui_preamble_count = 0;
        }
        
        switch (rcv_gui_state) {
            case read_type:
                ((char *) &rcv_gui_frame.header.type)[rcv_gui_byte_count] = b;
                rcv_gui_byte_count += 1;
                if (rcv_gui_byte_count == sizeof(byte_t)) {
                    // Dequeued timestamp, on to next
                    rcv_gui_state += 1;
                    rcv_gui_byte_count = 0;
                }
                break;
            case read_size:
                ((char *) &rcv_gui_frame.header.size)[rcv_gui_byte_count] = b;
                rcv_gui_byte_count += 1;
                if (rcv_gui_byte_count == sizeof(uint8_t)) {
                    // Dequeued size, and on to next
                    rcv_gui_state += 1;
                    rcv_gui_byte_count = 0;
                
                    // make space for payload
                    if (rcv_gui_frame.payload != NULL) {
                        // Clear old just in case
                        free(rcv_gui_frame.payload);
                    }
                    if (rcv_gui_frame.header.size > 0) {
                        // Allocate new
                        rcv_gui_frame.payload = malloc(rcv_gui_frame.header.size);
                    } else {
                        rcv_gui_frame.payload = NULL;
                    }
                }
                break;
            case read_payload:
                if (rcv_gui_frame.payload == NULL) {
                    rcv_gui_state += 1;
                    rcv_gui_byte_count = 0;
                } else {
                    rcv_gui_frame.payload[rcv_gui_byte_count] = b;
                    rcv_gui_byte_count += 1;
                    if (rcv_gui_byte_count == rcv_gui_frame.header.size) {
                        // Dequeued payload, on to next
                        rcv_gui_state += 1;
                        rcv_gui_byte_count = 0;
                    }
                }
                break;
            case read_checksum: 
                rcv_gui_frame.footer.interleaving = b;
                // Dequeued checksum, on to next
                rcv_gui_state += 1;
                break;
            default:
                // Ignore...
                break;
        }

        if (rcv_gui_state == read_complete) {
            frame_p duplicate = dupl_frame(&rcv_gui_frame);

            // Reset state for next frame
            rcv_gui_state = read_preamble;
            rcv_gui_preamble_count = 0;
            rcv_gui_byte_count = 0;
            rcv_gui_frame.header.type = 0;
            rcv_gui_frame.header.size = 0;
            free(rcv_gui_frame.payload);
            rcv_gui_frame.payload = NULL;
            rcv_gui_frame.footer.interleaving = 0;

            // Return frame if check matches, rest of queue can be done later
            if (duplicate->footer.interleaving == frame_to_interleaving(duplicate)) {
                // No errors detected, return frame. Rest of queue can be done later.
                return duplicate;
            } else {
                // Errors detected, throw away frame.
                destr_frame(duplicate);
            }
        }
    }

    return NULL;
}

void rcv_parameters(byte_p parameters)
{
    int8_t* message = (int8_t *) parameters;
    incoming_k_y = message[0];
    incoming_k_r_1 = message[1];
    incoming_k_r_2 = message[2];
    incoming_par_3 = message[3];
    incoming_par_4 = message[4];

    // Forward to GUI
    snd_parameters();
}

void rcv_control_mode(byte_p mode)
{
    // From GUI, forward to drone
    printf("Forwarding mode: %d\n", (control_mode_e) mode[0]);
    snd_control_mode((control_mode_e) mode[0]);
}

void rcv_key(byte_p key)
{
    // From GUI, throw into terminal key input
}

void snd_control_vector_gui(int16_t p_offset, int16_t pitch, int16_t r_offset, int16_t roll, int16_t y_offset, int16_t yaw, int16_t z_offset, int16_t lift)
{
    int16_t message[8] = { 
        p_offset, pitch / 256,
        r_offset, roll / 256,
        y_offset, yaw / 256,
        z_offset, lift / 256
    };	
    
    frame_p frame = new_frame((byte_p) &message, control_vector_gui_payload, control_vector_gui_size);
    comm_gui_snd_frame(frame);
    destr_frame(frame);
}