/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#ifndef _JS_H
#define _JS_H

#include "joystick.h"
#include "util.h"


// current axis and button readings
int16_t	axis[6];
int16_t	button[12];


int fd;
struct js_event js;


/**
 * Initialize the joystick.
 */
void js_init (char*);

/**
 * Read input from the joystick.
 * Values are stored in the 'axis' and 'button' fields.
 * 
 * Returns the number of processed events.
 */
int get_js_input(void);

#endif
