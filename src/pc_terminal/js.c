/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */

#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "js.h"

void js_init (char* device)
{
	if ((fd = open(device, O_RDONLY)) < 0) {
		fprintf(stderr, "\n[Joystick]\tError opening device ");
		perror(device);
		exit_pc_terminal(1);
	}

	// Non-blocking mode
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

int get_js_input(void)
{
	// Mvdr catcher
	#ifdef __APPLE__
	return 0;
	#else
	int result = 0;
	
	// Check up on JS
	while (read(fd, &js, sizeof(struct js_event)) == 
					sizeof(struct js_event))  {
		result++;
		
		// Register data
		switch(js.type & ~JS_EVENT_INIT) {
			case JS_EVENT_BUTTON:
				button[js.number] = js.value;
				break;
			case JS_EVENT_AXIS:
				axis[js.number] = js.value;
				break;
		}
	}
	
	if (errno != EAGAIN) 
	{
		perror("\n[Joystick]\tError reading (EAGAIN)");
		exit_pc_terminal(1);
	}
	
	return result;
	#endif
}
