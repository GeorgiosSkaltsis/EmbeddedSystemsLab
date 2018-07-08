/**
 * comm_terminal.h
 * 
 * defines and function prototypes for pc-side message receival/sending
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef PC_COMM_TERMINAL_H
#define PC_COMM_TERMINAL_H

#include <inttypes.h>
#include "../shared/types.h"
#include "../shared/values.h"
#include "../shared/comm_protocol.h"
#include "gui_server.h"
#include <time.h>

byte_t timestamp_drone;
control_mode_e outgoing_control_mode;
control_mode_e incoming_control_mode;
byte_queue_t gui_rcv_queue;

void snd_control_vector(int16_t pitch, int16_t roll, int16_t yaw, int16_t lift);
void snd_control_mode(control_mode_e mode);
void snd_parameters();
void snd_abort_or_exit();
void rcv_log(byte_p bytes, uint8_t size);
void rcv_info(byte_p bytes);
void rcv_battery_low();
void snd_frame_direct(frame_p frame);
void snd_ping();
void snd_ping_gui(clock_t time_diff);
void rcv_ping();

#endif // PC_COMM_TERMINAL_H
