/**
 * comm_protocol.h
 * 
 * Function implementation for communication protocol, shared by both sides.
 * 
 * M. van de Ruit
 *
 * May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include "comm_protocol.h"

void comm_protocol_init()
{
    constr_byte_queue(&rcv_queue);
}

// Incoming frame state
enum {
    read_preamble,
    read_type,
    read_size,
    read_payload,
    read_checksum,
    read_complete
} rcv_tmp_state = read_preamble;
frame_t rcv_tmp_frame;
uint16_t rcv_tmp_preamble_count = 0;
uint16_t rcv_tmp_byte_count = 0;

frame_p rcv_frame() 
{
    // Empty queue of incoming bytes
    while (rcv_queue.count > 0) {
        byte_t b = byte_dequeue(&rcv_queue);

        // Check for preamble
        if (b == 'a') {
            rcv_tmp_preamble_count += 1;
            if (rcv_tmp_preamble_count == preamble_len) {
                // We read a preamble, restart read
                rcv_tmp_state = read_preamble + 1;
                rcv_tmp_preamble_count = 0;
                rcv_tmp_byte_count = 0;
                continue;
            }
        } else {
            rcv_tmp_preamble_count = 0;
        }
        
        switch (rcv_tmp_state) {
            case read_type:
                ((char *) &rcv_tmp_frame.header.type)[rcv_tmp_byte_count] = b;
                rcv_tmp_byte_count += 1;
                if (rcv_tmp_byte_count == sizeof(byte_t)) {
                    // Dequeued timestamp, on to next
                    rcv_tmp_state += 1;
                    rcv_tmp_byte_count = 0;
                }
                break;
            case read_size:
                ((char *) &rcv_tmp_frame.header.size)[rcv_tmp_byte_count] = b;
                rcv_tmp_byte_count += 1;
                if (rcv_tmp_byte_count == sizeof(uint8_t)) {
                    // Dequeued size, and on to next
                    rcv_tmp_state += 1;
                    rcv_tmp_byte_count = 0;
                
                    // make space for payload
                    if (rcv_tmp_frame.payload != NULL) {
                        // Clear old just in case
                        free(rcv_tmp_frame.payload);
                    }
                    if (rcv_tmp_frame.header.size > 0) {
                        // Allocate new
                        rcv_tmp_frame.payload = malloc(rcv_tmp_frame.header.size);
                    } else {
                        rcv_tmp_frame.payload = NULL;
                    }
                }
                break;
            case read_payload:
                if (rcv_tmp_frame.payload == NULL) {
                    rcv_tmp_state += 1;
                    rcv_tmp_byte_count = 0;
                } else {
                    rcv_tmp_frame.payload[rcv_tmp_byte_count] = b;
                    rcv_tmp_byte_count += 1;
                    if (rcv_tmp_byte_count == rcv_tmp_frame.header.size) {
                        // Dequeued payload, on to next
                        rcv_tmp_state += 1;
                        rcv_tmp_byte_count = 0;
                    }
                }
                break;
            case read_checksum: 
                rcv_tmp_frame.footer.interleaving = b;
                // Dequeued checksum, on to next
                rcv_tmp_state += 1;
                break;
            default:
                // Ignore...
                break;
        }

        if (rcv_tmp_state == read_complete) {
            frame_p duplicate = dupl_frame(&rcv_tmp_frame);

            // Reset state for next frame
            rcv_tmp_state = read_preamble;
            rcv_tmp_preamble_count = 0;
            rcv_tmp_byte_count = 0;
            rcv_tmp_frame.header.type = 0;
            rcv_tmp_frame.header.size = 0;
            free(rcv_tmp_frame.payload);
            rcv_tmp_frame.payload = NULL;
            rcv_tmp_frame.footer.interleaving = 0;

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



