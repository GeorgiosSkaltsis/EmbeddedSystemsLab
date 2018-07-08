/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#ifndef _SERIAL_H
#define _SERIAL_H

#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "util.h"

int fd_RS232;

/*------------------------------------------------------------
 * Serial I/O
 * 8 bits, 1 stopbit, no parity,
 * 115,200 baud
 *------------------------------------------------------------
 */
void rs232_open(void);
void rs232_close(void);
int rs232_getchar_nb();
int rs232_getchar();
int rs232_putchar(char c);

#endif
