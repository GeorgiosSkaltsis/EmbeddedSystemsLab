/**
 * comm_terminal.c
 * 
 * implementation of pc-side communication receival/sending
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include <stdlib.h>
#include "serial.h"
#include "../shared/values.h"
#include "comm_terminal.h"
#include <time.h>

clock_t ping_start;
byte_t timestamp_drone = 0;
control_mode_e outgoing_control_mode = safe_mode;

void snd_parameters()
{
    int8_t message[5] = {
        incoming_k_y,
        incoming_k_r_1,
        incoming_k_r_2,
        incoming_par_3,
        incoming_par_4
    };
    
    frame_p frame = new_frame((byte_p) &message, parameter_payload, parameter_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void snd_control_vector(int16_t pitch, int16_t roll, int16_t yaw, int16_t lift)
{
    int16_t message[4] = { 
        pitch / 256, 
        roll / 256, 
        yaw / 256, 
        lift / 256 
    };	
    
    frame_p frame = new_frame((byte_p) &message, control_vector_payload, control_vector_size);
    snd_frame_direct(frame);
    comm_gui_snd_frame(frame);
    destr_frame(frame);
}

void snd_control_mode(control_mode_e mode)
{
    outgoing_control_mode = mode;

    frame_p frame = new_frame((byte_p) &mode, control_mode_payload, control_mode_size);
    if (mode == panic_mode)
    {
        for (unsigned i = 0; i < 5; i++) {
            snd_frame_direct(frame);
        }
    } 
    else
    {
        snd_frame_direct(frame);
    }
    destr_frame(frame);
}

void snd_abort_or_exit()
{
    printf("Sending abort or exit\n");
    byte_p empty = "x";
    frame_p frame = new_frame(empty, abort_or_exit_payload, abort_or_exit_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void rcv_log(byte_p bytes, uint8_t size)
{
    printf("Received log: [ %.*s ]\n", size, bytes);
}

void rcv_info(byte_p bytes)
{
    printf("Received info: [ %.*s ]\n", info_size, bytes);
    incoming_control_mode = bytes[info_size - 1] - '0';
    
    // Forward to GUI
    frame_p frame = new_frame(bytes, info_payload, info_size);
    comm_gui_snd_frame(frame);
    destr_frame(frame);
}

void rcv_battery_low()
{
    printf("Warning: battery low.\n");
}

void snd_ping()
{
    // Start measuring time delay
    ping_start = clock();
    frame_p frame = new_frame("x", ping_payload, ping_payload_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void rcv_ping()
{
    // End measuring time delay
    clock_t ping_end = clock();
    clock_t difference = ping_end - ping_start;
    snd_ping_gui(difference);
}

void snd_ping_gui(clock_t time_diff)
{
    byte_p bytes = (byte_p) &time_diff;
    frame_p frame = new_frame(bytes, ping_payload, sizeof(clock_t));
    comm_gui_snd_frame(frame);
    destr_frame(frame);
}

void comm_protocol_update_rcv()
{    
    // Store incoming byte in receive queue
    char c;
    if ((c = rs232_getchar_nb()) != -1) 
    {
        byte_enqueue(&rcv_queue, c);
    }

    // Empty queue looking for frames
    frame_p frame = rcv_frame();
    if (frame != NULL) 
    {
        switch(frame->header.type) 
        {
            case log_payload:
                rcv_log(frame->payload, frame->header.size);
                break;
            case info_payload:
                rcv_info(frame->payload);
                break;
            case battery_low_payload:
                rcv_battery_low();
                break;
            case ping_payload:
                rcv_ping();
                break;
            default:
                // ignore
                break;
        }
        destr_frame(frame);
    }
}

void snd_frame_direct(frame_p frame)
{
    for (uint8_t i = 0; i < sizeof(frame_hdr_t); i++) {
        rs232_putchar(((byte_p) &frame->header)[i]);
    }
    for (uint8_t i = 0; i < frame->header.size; i++) {
        rs232_putchar(frame->payload[i]);
    }
    for (uint8_t i = 0; i < sizeof(frame_ftr_t); i++) {
        rs232_putchar(((byte_p) &frame->footer)[i]);
    }
}
