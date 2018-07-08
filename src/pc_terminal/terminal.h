/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "util.h"

struct termios 	savetty;

void term_initio();
void term_exitio();
void term_puts(char *s);
void term_putchar(char c);
int	term_getchar_nb();
int	term_getchar();

void term_init_header();

#endif
