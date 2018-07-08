#include "pc_terminal.h"
#include "js.h"
#include "terminal.h"
#include "serial.h"
#include "util.h"
#include "../shared/comm_protocol.h" // TODO: hack, fix
#include "comm_terminal.h"
#include "gui_server.h"
#include "ping_timer.h"

int main(int argc, char **argv)
{
	char c;

	term_init_header();

	term_initio();
	rs232_open();
	comm_protocol_init();
	server_init();
	ping_timer_init();
	
#ifdef JS_DEV
	js_init(JS_DEV);
#endif

	// Discard any incoming text
	while ((c = rs232_getchar_nb()) != -1) 
		fputc(c, stderr);

	for (;;)
	{
		/*
		 * Whether to send any new values to the drone.
		 * TODO: Replace this with a timed event
		 */
		int sendUpdate = 0;
		
		while ((c = term_getchar_nb()) != -1) {
			if (process_key(c))
				sendUpdate |= 1;
			else
				break;
		}

		ping_timer_update();
		
#ifdef JS_DEV
		if (get_js_input()) 
		{
			// If trigger triggered
			if (button[0])
			{
				snd_abort_or_exit();
			}
			sendUpdate |= 1;
		}
#endif

		if (sendUpdate) {
			calc_control_vector(p_offset, r_offset, y_offset, z_offset);
		}

		// Read incoming data
		comm_gui_update_rcv();
		comm_protocol_update_rcv();
	}

	sleep(1);
	comm_protocol_update_rcv();
	printf("Shut down successfully.\n");
	exit_pc_terminal(0);
}
