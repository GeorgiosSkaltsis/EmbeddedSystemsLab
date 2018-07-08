/*
 * Copied from the original 'pc_terminal.c', with some small additions.
 * 
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#include "serial.h"

void rs232_open(void)
{
  	char *name;
  	int result;
  	struct termios tty;
#ifdef SERIAL_PORT
    fd_RS232 = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK);
#else
    fd_RS232 = open("undefined", O_RDWR | O_NOCTTY | O_NONBLOCK);
#endif

	assert(fd_RS232>=0, "Error opening serial connection");

  	result = isatty(fd_RS232);
  	assert(result == 1, "Error opening serial connection");

  	name = ttyname(fd_RS232);
  	assert(name != 0, "Error opening serial connection");

  	result = tcgetattr(fd_RS232, &tty);
	assert(result == 0, "Error opening serial connection");

	tty.c_iflag = IGNBRK; /* ignore break condition */
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 1; // added timeout

	tty.c_iflag &= ~(IXON|IXOFF|IXANY);

	result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */

	tcflush(fd_RS232, TCIOFLUSH); /* flush I/O buffer */
}


void rs232_close(void)
{
  	int result;

  	result = close(fd_RS232);
  	assert(result==0, "Error closing serial connection");
}

int	rs232_getchar_nb()
{
	int result;
	unsigned char 	c;

	result = read(fd_RS232, &c, 1);

	if (result <= 0)
		return -1;
	else
	{
		assert(result == 1, "Error reading serial data");
		return (int) c;
	}
}


int rs232_getchar()
{
	int c;

	while ((c = rs232_getchar_nb()) == -1);
	return c;
}


int rs232_putchar(char c)
{
	int result;

	do {
		result = (int) write(fd_RS232, &c, 1);
	} while (result == 0);

	assert(result == 1, "Error writing serial data");
	return result;
}
