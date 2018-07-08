#ifndef CONTROL_H__
#define CONTROL_H__

#include <stdbool.h>
#include <inttypes.h>

#define MOTOR_MIN 200
#define MOTOR_MAX 650

/**
 * This function detects values of ae outside the limits (ae belongs to (0,limit)) and saturates their values.
 */
void limit_motors(int16_t min, int16_t max);

/**
 * Update the engine values and apply default limiting.
 */
void update_motors();

/**
 * Update the engine values and apply the given limiting.
 */
void update_motors_range(int16_t min, int16_t max);

/**
 * Check whether the motors are idle.
 */
bool motors_off();

#endif // CONTROL_H__
