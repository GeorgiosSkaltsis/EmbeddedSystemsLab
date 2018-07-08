/*------------------------------------------------------------------
 *  control.c -- here you can implement your control algorithm
 *		 and any motor clipping or whatever else
 *		 remember! motor input =  0-1000 : 125-250 us (OneShot125)
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  July 2016
 *------------------------------------------------------------------
 */

#include <inttypes.h>
#include "in4073.h"
#include "control.h"

/**
 * Author: Chiel Bruin
 * Editor: Georgios Skaltsis, added lower bound case.
 */
void limit_motors(int16_t min, int16_t max)
{
	ae[0] = ae[0] > max ? max : ae[0];
	ae[1] = ae[1] > max ? max : ae[1];
	ae[2] = ae[2] > max ? max : ae[2];
	ae[3] = ae[3] > max ? max : ae[3];

	ae[0] = ae[0] < min ? min : ae[0];
	ae[1] = ae[1] < min ? min : ae[1];
	ae[2] = ae[2] < min ? min : ae[2];
	ae[3] = ae[3] < min ? min : ae[3];
}

/**
 * Author: Chiel Bruin
 */
void update_motors_range(int16_t min, int16_t max)
{
	limit_motors(min, max);
	motor[0] = (int16_t) ae[0];
	motor[1] = (int16_t) ae[1];
	motor[2] = (int16_t) ae[2];
	motor[3] = (int16_t) ae[3];
}

/**
 * Author: Chiel Bruin
 */
void update_motors()
{
	update_motors_range(MOTOR_MIN, MOTOR_MAX);
}

/**
 * Author: Chiel Bruin
 */
bool motors_off() {
	return
		motor[0] == 0 &&
		motor[1] == 0 &&
		motor[2] == 0 &&
		motor[3] == 0;
}
