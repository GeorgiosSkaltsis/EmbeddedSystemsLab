/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "terminal.h"
#include "serial.h"

/**
 * Close the pc-terminal with the given exitcode.
 */
void exit_pc_terminal(int code);

/**
 * Make an assertion.
 * When the assertion fails, the provided message is shown
 * and the terminal is closed.
 */
void assert(int test, char* msg);

/**
 * Apply an offset to a value clipping any overflow.
 */
int16_t safe_offset(int16_t value, int16_t offset);

/**
 * Calculate the correct lift value.
 * This value is always in the range [0, INT16_MAX] and does not overflow.
 */
int16_t calculate_lift(int16_t axis, int16_t offset);

#endif
