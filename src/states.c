#include "states.h"
#include "control.h"
#include "comm_drone.h"
#include "in4073.h"

control_mode_e current_mode = safe_mode;
int32_t desired_pressure;

/**
 * Author: Chiel Bruin
 * Edited: Dhaval Shah. (Added Calibration)
 */
void transition_to(control_mode_e mode) 
{
    switch (mode)
	{
		case raw_mode:
#ifndef TESTING
            imu_init(false, 400);
#endif
			mode_step_calibration();
			break;
        case height_mode:
            desired_pressure = pressure;
			break;
		
		default: break;
	}
}

/**
 * Author: Chiel Bruin
 */
void transition_from(control_mode_e mode)
{
    switch (mode)
	{
		case raw_mode:
#ifndef TESTING
            imu_init(true, 100);
#endif
			break;
		
		default: break;
	}
}

/**
 * Author: Chiel Bruin
 */
void mode_step(control_mode_e mode)
{
	int16_t roll  = incoming_roll;
	int16_t pitch = incoming_pitch;
	int16_t yaw   = incoming_yaw;
	int16_t lift  = incoming_lift;
	
	// If the battery is empty, switch to panic mode
	if (bat_volt < BAT_MIN && mode != safe_mode) 
	{
		mode = panic_mode;
	}
  
	if (mode != current_mode && transition_is_safe_to(mode))
	{
        transition_from(current_mode);
        transition_to(mode);
		current_mode = mode;
	}
	
	switch (current_mode)
	{
		case safe_mode:
			mode_step_safe();
			update_motors_range(0, 0);
			return;
		case panic_mode:
			mode_step_panic();
			update_motors_range(0, MOTOR_MAX);
			return;
		case manual_mode:
			mode_step_manual(roll, pitch, yaw, lift); 
			break;
		case calibrate_mode:
			mode_step_calibration();
			update_motors_range(0, 0);
			return;
		case yaw_control_mode:
			mode_step_yaw_control(roll, pitch, yaw, lift);
			break;
		case full_control_mode:
			mode_step_full_control(roll, pitch, yaw, lift);
			break;
		 case height_mode:
            mode_step_height_control(roll, pitch, yaw, lift);
		 	break;
		case raw_mode:
			mode_step_raw(roll, pitch, yaw, lift);
		 	break;
		// case wireless_mode:
		// 	break;
		default:
			current_mode = panic_mode;
			mode_step_panic();
			break;
	}
    if (incoming_lift == 0) {
        update_motors_range(0, 0);
    } else {
        update_motors();
    }
}

/**
 * Author: Chiel Bruin
 */
bool transition_is_safe_to(control_mode_e new_mode)
{
	// Staying in the same mode is always safe
	if (new_mode == current_mode) return true;
	
	// Switching to panic mode is always safe
	if (new_mode == panic_mode) return true;
	
	switch (current_mode)
	{
		case safe_mode:
			// You may only exit safe mode when the engines are off
			return incoming_lift == 0;
		case panic_mode:
			return new_mode == safe_mode && motors_off();
		case manual_mode:
			return new_mode == safe_mode && motors_off();
		case calibrate_mode:
			return new_mode == safe_mode && motors_off();
		case yaw_control_mode:
			return new_mode == safe_mode && motors_off();
		case full_control_mode:
			return (new_mode == safe_mode && motors_off()) || (new_mode == height_mode);
		case raw_mode:
            return new_mode == safe_mode && motors_off();
        case height_mode:
			return (new_mode == safe_mode && motors_off()) || (new_mode == full_control_mode);
		// case wireless_mode:
		// 	return false;
		default:
			return false;
	}
	
}

/**
 * Author: Chiel Bruin
 */
void mode_step_panic()
{
	// Do not spin down every iteration, thats way too fast
	// This makes it update at 20Hz, so 400 -> 0 takes 2 seconds
	if (check_timer_flag()) {
		for (uint8_t i = 0; i < 4; i++)
		{
			// Negative bounds checking is offloaded to limit_motors, saving some processing time
			ae[i] -= 10;
		}
	}
}

/**
 * Author: Chiel Bruin
 */
void mode_step_safe()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		ae[i]= 0;
	}
}

/**
 * Author: Georgios Skaltsis
 */
void mode_step_manual(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift){
	// TODO: We should define b and d. I guess by trial and error, as long as we don't have given values. (Random values have been assigned above..)
	// We assume that we receive the signals roll, pitch, yaw and lift
	const int16_t b = 2;
	const int16_t d = 2;
	
	const int16_t tb = b; 
	const int16_t fb = 2 * b;
	const int16_t fd = d;
	
	ae[0] = lift * fb + pitch * tb - yaw * fd; //Watch out for integers division!!!
	ae[1] = lift * fb - roll * tb + yaw * fd;
	ae[2] = lift * fb - pitch * tb - yaw * fd;
	ae[3] = lift * fb + roll * tb + yaw * fd;
}

/**
 * Author: Georgios Skaltsis
 * Edited: Dhaval Shah. Fixed the equation
 * Edited: Chiel Bruin, made changes to calculate in 32-bit
 */
void mode_step_yaw_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
	// TODO: Define b and d.
	const int32_t b = 2;
	const int32_t d = 2;
	
	const int32_t tb=b; 
	const int32_t fb=2*b;
	const int32_t fd=d;
	
	yaw_n = incoming_k_y * (yaw / 32 - ((int32_t) sr-sr_offset) / 128);  // sr is the angular rate of the respective axis.
	
	ae[0] = lift * fb + pitch * tb - yaw_n * fd; //Watch out for integers division!!!
	ae[1] = lift * fb - roll * tb + yaw_n * fd;
	ae[2] = lift * fb - pitch * tb - yaw_n * fd;
	ae[3] = lift * fb + roll * tb + yaw_n * fd;
}

/**
 * Author: Georgios Skaltsis
 * Edited: Chiel Bruin, made changes to calculate in 32-bit
 */
void mode_step_full_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
	// TODO: Define b and d.
	const int32_t b = 2;
	const int32_t d = 2;
	
	const int32_t tb=b; 
	const int32_t fb=2*b;
	const int32_t fd=d;
	
	yaw_n = incoming_k_y * (yaw / 32 - ((int32_t) sr - sr_offset) / 128);  // sr is the angular rate of the respective axis.
	roll_n = incoming_k_r_1 * (roll / 16 -((int32_t) phi - phi_offset) / 1024) - incoming_k_r_2 * ((int32_t) sp - sp_offset) / 1024;
	pitch_n = incoming_k_r_1 * (pitch / 16 - ((int32_t) theta - theta_offset) / 1024)  + incoming_k_r_2 * ((int32_t) sq - sq_offset) / 1024; // We, maybe temporarily use the same gains for roll and pitch.

	//pitch = 0;

	ae[0] = lift * fb + pitch_n * tb - yaw_n * fd; //Watch out for integers division!!!
	ae[1] = lift * fb - roll_n * tb + yaw_n * fd;
	ae[2] = lift * fb - pitch_n * tb - yaw_n * fd;
	ae[3] = lift * fb + roll_n * tb + yaw_n * fd;
}

/**
 * Author: Georgios Skaltsis
 */
void mode_step_height_control(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
	lift =lift + incoming_par_3 * (pressure - desired_pressure) / 2;
	mode_step_full_control(roll, pitch, yaw, lift);
}

/**
 * Author: Georgios Skaltsis
 * Edited: Chiel Bruin (Made sure engines are off)
 */
void mode_step_calibration()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		ae[i] = 0;
	}
		
	//TODO: Put some filter over this to eliminate noise
	phi_offset = phi;
	theta_offset = theta;
	psi_offset = psi;
	
	sq_offset = sq;
	sp_offset = sp;
	sr_offset = sr;
}


/**
 * Author: Dhaval Shah
 */
void mode_step_raw(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
//	mode_step_calibration();
	mode_step_full_control(roll, pitch, yaw, lift);

}
