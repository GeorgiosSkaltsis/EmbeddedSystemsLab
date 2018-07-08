#ifndef STATES_H__
#define STATES_H__

#include <inttypes.h>
#include <stdbool.h>
#include "types.h"

// Proportional gain of the controller.
//TODO: We need to define the regulation of that value in the protocol!
int16_t K_p ;
int32_t desired_pressure;
int32_t pitch_n, roll_n, yaw_n;
control_mode_e current_mode;
	
/**
 * Step one round of calculations for the specified mode.
 */
void mode_step(control_mode_e mode);

/**
 * Check if the provided new state is a valid transition.
 * Returns true when the transition is valid. 
 */
bool transition_is_safe_to(control_mode_e mode);

/**
 * Called when transitioning to the given state.
 */
void transition_to(control_mode_e mode);

/**
 * Called when transitioning from the given state.
 */
void transition_from(control_mode_e mode);

/**
 * Run the panic mode.
 * Gently spins down the engines.
 */
void mode_step_panic ();

/**
 * Run the safe mode.
 * Idles the drone.
 */
void mode_step_safe ();

/**
 * Runs calibration control mode.
 */
void mode_step_calibration();

/**
 * Runs full control mode.
 */
void mode_step_full_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);

/**
 * This function calculates the corresponding values of the voltages(or RPMs) of the motors, 
 * for the given (transmitted) values of roll, pitch, yaw and lift.
 * This is done in a way that gives the pilot direct (manual) control over the drones movement.
 */
void mode_step_manual(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);

/**
 * This function calculates the corresponding values of the voltages(or RPMs) of the motors, 
 * for the given (transmitted) values of roll, pitch, yaw and lift, for the Yaw Mode.
 */
void mode_step_yaw_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);

/**
 * This function calculates the corresponding values of the voltages(or RPMs) of the motors, 
 * for the given (transmitted) values of roll, pitch, yaw and lift, for the Height Mode.
 */
void mode_step_height_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);

/**
 * This function calculates the corresponding values of the voltages(or RPMs) of the motors, 
 * for the given (transmitted) values of roll, pitch, yaw and lift, for the Raw Mode.
 */
void mode_step_raw(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);


#endif
