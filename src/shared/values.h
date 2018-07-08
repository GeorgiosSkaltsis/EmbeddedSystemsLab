/**
 * values.h
 * 
 * defines, types for shared values
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef SHARED_VALUES_H
#define SHARED_VALUES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include "types.h"

// Shared control mode
control_mode_e incoming_control_mode;
bool should_abort_or_exit;

// Shared control parameters
int16_t incoming_pitch;
int16_t incoming_roll;
int16_t incoming_yaw;
int16_t incoming_lift;

// Shared tweak parameters (and keyboard controls)
int8_t incoming_k_y;        // +Z/-X
int8_t incoming_k_r_1;      // +C/-V
int8_t incoming_k_r_2;      // +B/-N
int8_t incoming_par_3;      // +M/-,
int8_t incoming_par_4;      // +./-/

#ifdef __cplusplus
};
#endif

#endif // SHARED_VALUES_H