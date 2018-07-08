/**
 * comm_drone.c
 * 
 * implementation of drone-side communication receival/sending
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "comm_drone.h"
#include "states.h"

uint16_t timeout_counter = timeout_counter_max;
byte_t timestamp_pc = 0;

void snd_log(byte_p log, uint8_t size)
{
    frame_p frame = new_frame(log, log_payload, size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void snd_info(byte_p info)
{
    frame_p frame = new_frame(info, info_payload, info_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void snd_battery_low()
{
    frame_p frame = new_frame("x", battery_low_payload, battery_low_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void rcv_control_vector(byte_p vector)
{
    int16_t* message = (int16_t *) vector;
    incoming_pitch = message[0];
    incoming_roll = message[1];
    incoming_yaw = message[2];
    incoming_lift = message[3];
}

void rcv_control_mode(byte_p mode)
{
    incoming_control_mode = (control_mode_e) mode[0]; 
}

void rcv_abort_or_exit()
{
    should_abort_or_exit = true;
}

void rcv_ping()
{
    snd_ping();
}

void snd_ping()
{
    frame_p frame = new_frame("x", ping_payload, ping_payload_size);
    snd_frame_direct(frame);
    destr_frame(frame);
}

void rcv_parameters(byte_p parameters)
{
    int8_t* message = (int8_t *) parameters;
    incoming_k_y = message[0];
    incoming_k_r_1 = message[1];
    incoming_k_r_2 = message[2];
    incoming_par_3 = message[3];
    incoming_par_4 = message[4];
}

void comm_protocol_update_rcv()
{
    while (rcv_queue.count > 0) {
        frame_p frame = rcv_frame();
        if (frame != NULL) {
            switch(frame->header.type) {
                case control_vector_payload:
                    rcv_control_vector(frame->payload);
                    break;
                case control_mode_payload:
                    rcv_control_mode(frame->payload);
                    break;
                case abort_or_exit_payload:
                    rcv_abort_or_exit();
                    break;
                case parameter_payload:
                    rcv_parameters(frame->payload);
                    break;
                case ping_payload:
                    rcv_ping();
                    break;
                default:
                    // ignore
                    break;
            }
            timeout_counter = timeout_counter_max; // Received message, cancel apocalypse
            destr_frame(frame);
        }
    }
}

void snd_frame_direct(frame_p frame)
{
    for (uint8_t i = 0; i < sizeof(frame_hdr_t); i++) {
        fputc(((byte_p) &frame->header)[i], stdout);
    }
    for (uint8_t i = 0; i < frame->header.size; i++) {
        fputc(frame->payload[i], stdout);
    }
    for (uint8_t i = 0; i < sizeof(frame_ftr_t); i++) {
        fputc(((byte_p) &frame->footer)[i], stdout);
    }
    fputc('\n', stdout);
}
