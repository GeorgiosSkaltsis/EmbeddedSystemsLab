/**
 * comm_drone.h
 * 
 * defines and function prototypes for drone-side message receival/sending
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef DRONE_COMM_DRONE_H
#define DRONE_COMM_DRONE_H

#include <stdbool.h>
#include <inttypes.h>
#include "../shared/types.h"
#include "../shared/values.h"
#include "comm_protocol.h"

#define timeout_counter_max 5; // 5 ticks at 20hz, so 250 ms

// Keep-running values
byte_t timestamp_pc;
uint16_t timeout_counter;

void snd_info(byte_p info);
void snd_log(byte_p log, uint8_t size);
void snd_battery_low();
void rcv_control_vector(byte_p vector);
void rcv_control_mode(byte_p mode);
void rcv_abort_or_exit();
void rcv_parameters(byte_p parameters);
void rcv_ping();
void snd_ping();
void snd_frame_direct(frame_p frame);

#endif // DRONE_COMM_DRONE_H
