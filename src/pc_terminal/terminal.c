/*
 * Copied from the original 'pc_terminal.c', with some small additions.
 * 
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#include "terminal.h"

void term_initio()
{
	struct termios tty;

	tcgetattr(0, &savetty);
	tcgetattr(0, &tty);

	tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;

	tcsetattr(0, TCSADRAIN, &tty);
}

void term_exitio()
{
	tcsetattr(0, TCSADRAIN, &savetty);
}

void term_puts(char *s)
{
	fprintf(stderr,"%s",s);
}

void term_putchar(char c)
{
	putc(c,stderr);
}

int	term_getchar_nb()
{
	static unsigned char 	line [2];

	if (read(0,line,1)) // note: destructive read
		return (int) line[0];

	return -1;
}

int	term_getchar()
{
	int c;

	while ((c = term_getchar_nb()) == -1);
	return c;
}

void term_init_header()
{
	term_puts("\n    ______           _              _     _          _        ");
	term_puts("\n   |  ____|         | |            | |   | |        | |       ");
	term_puts("\n   | |__   _ __ ___ | |__   ___  __| | __| | ___  __| |       ");
	term_puts("\n   |  __| | '_ ` _ \\| '_ \\ / _ \\/ _` |/ _` |/ _ \\/ _` |   ");
	term_puts("\n   | |____| | | | | | |_) |  __/ (_| | (_| |  __/ (_| |       ");
	term_puts("\n   |______|_| |_| |_|_.__/ \\___|\\__,_|\\__,_|\\___|\\__,_|  ");
	term_puts("\n                                                              ");
	term_puts("\n     _____           _                           _           _           ");
	term_puts("\n    / ____|         | |                         | |         | |          ");
	term_puts("\n   | (___  _   _ ___| |_ ___ _ __ ___  ___      | |     __ _| |__        ");
	term_puts("\n    \\___ \\| | | / __| __/ _ \\ '_ ` _ \\/ __|     | |    / _` | '_ \\  ");
	term_puts("\n    ____) | |_| \\__ \\ ||  __/ | | | | \\__ \\     | |___| (_| | |_) |  ");
	term_puts("\n   |_____/ \\__, |___/\\__\\___|_| |_| |_|___/     |______\\__,_|_.__/   ");
	term_puts("\n            __/ |                                                        ");
	term_puts("\n           |___/        Mission Control [V0.1]                           ");
                          
	term_puts("\n\nGroup 13:");
	term_puts("\n -Chiel Bruin");
	term_puts("\n -Dhaval Shah");
	term_puts("\n -Georgios Skaltsis");
	term_puts("\n -Mark van de Ruit");
	
	term_puts("\n\nType <ESC> to exit\n");
}
