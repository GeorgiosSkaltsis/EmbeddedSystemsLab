/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */
#ifndef _PC_TERMINAL_H
#define _PC_TERMINAL_H

#include <inttypes.h>

int p_offset, r_offset, y_offset, z_offset;
int sensitivity;

/**
 * Calculates the control vector to send to the drone from the joystick values.
 * Applies the offsets as given to the function.
 */
void calc_control_vector(int16_t p_offset, int16_t r_offset, int16_t y_offset, int16_t z_offset);
// TODO: Make this return the protocol vector struct

/**
 * Process the given input.
 */
int process_key(uint8_t c);

#endif
