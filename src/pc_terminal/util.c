/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#include "util.h"

/**
 * Author: Chiel Bruin
 */
void exit_pc_terminal(int code)
{
	term_exitio();
	if (fd_RS232>=0) rs232_close();
	term_puts("\n<exit>\n");
	exit(code);
}

/**
 * Author: Chiel Bruin
 */
void assert(int test, char* msg)
{
	if (!test)
	{
		fprintf(stderr, "\n[Assert]\t%s", msg);
		exit_pc_terminal(1);
	}
}

/**
 * Author: Chiel Bruin
 */
int16_t safe_offset(int16_t value, int16_t offset) {
	int16_t result = value + offset;

	// Check overflow
	if ((result < value) && (offset > 0))
	{
		result = INT16_MAX;
	}
	// Check underflow
	else if ((result > value) && (offset < 0))
	{
		result = INT16_MIN;
	}

	return result;
}

/**
 * Author: Chiel Bruin
 */
int16_t calculate_lift(int16_t axis, int16_t offset)
{
	int16_t res = safe_offset(axis/2 + 16384, offset);
	return res > 0 ? res : 0;
}
