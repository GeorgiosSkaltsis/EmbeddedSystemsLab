/**
 * values.
 * 
 * implementation for shared values
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "values.h"

control_mode_e incoming_control_mode = safe_mode;
bool should_abort_or_exit = false;

int16_t incoming_pitch = 0;
int16_t incoming_roll = 0;
int16_t incoming_yaw = 0;
int16_t incoming_lift = 0;

int8_t incoming_k_y = 16;
int8_t incoming_k_r_1 = 9;
int8_t incoming_k_r_2 = 23;
int8_t incoming_par_3 = 10;
int8_t incoming_par_4 = 0;