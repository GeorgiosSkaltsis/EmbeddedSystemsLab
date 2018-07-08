/*
 * Author: Chiel Bruin
 * Date: 8-5-2018
 */
#include "pc_terminal.h"
#include "js.h"
#include "terminal.h"
#include "util.h"
#include "../shared/comm_protocol.h" // TODO: hack, fix
#include "comm_terminal.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

int p_offset, r_offset, y_offset, z_offset = 0;
int sensitivity = 500;
int16_t old_lift = 0;

int process_key(uint8_t c)
{
	switch (c)
	{
		case '0':
			snd_control_mode(safe_mode);
			break;
		case '1':
			snd_control_mode(panic_mode);
			break;
		case '2':
			snd_control_mode(manual_mode);
			break;
		case '3':
			snd_control_mode(calibrate_mode);
			break;
		case '4':
			snd_control_mode(yaw_control_mode);
			break;
		case '5':
			snd_control_mode(full_control_mode);
			break;
		case '6':
			snd_control_mode(raw_mode);
			break;
		case '7':
			snd_control_mode(height_mode);
			break;
		// case '8':
		// 	snd_control_mode(wireless_mode);
		// 	break;
		case 'A':
			p_offset = safe_offset(p_offset,  sensitivity);
			break;
		case 'B':
			p_offset = safe_offset(p_offset, -sensitivity);
			break;
		case 'D':
			r_offset = safe_offset(r_offset,  sensitivity);
			break;
		case 'C':
			r_offset = safe_offset(r_offset, -sensitivity);
			break;
		case 'w':
			y_offset = safe_offset(y_offset,  sensitivity);
			break;
		case 'q':
			y_offset = safe_offset(y_offset, -sensitivity);
			break;
		case 'a':
			z_offset = safe_offset(z_offset,  sensitivity);
			break;
		case 'z':
			z_offset = safe_offset(z_offset, -sensitivity);
			break;
		case 'u':
			incoming_k_y += 1;
			snd_parameters();
			break;
		case 'j':
			incoming_k_y = max(incoming_k_y - 1, 0);
			snd_parameters();
			break;
		case 'i':
			incoming_k_r_1 += 1;
			snd_parameters();
			break;
		case 'k':
			incoming_k_r_1 = max(incoming_k_r_1 - 1, 0);
			snd_parameters();
			break;
		case 'o':
			incoming_k_r_2 += 1;
			snd_parameters();
			break;
		case 'l':
			incoming_k_r_2 = max(incoming_k_r_2 - 1, 0);
			snd_parameters();
			break;
		case 'm':
			incoming_par_3 += 1;
			snd_parameters();
			break;
		case ',':
			incoming_par_3 = max(incoming_par_3 - 1, 0);
			snd_parameters();
			break;
		case '.':
			incoming_par_4 += 1;
			snd_parameters();
			break;
		case '/':
			incoming_par_4 = max(incoming_par_4 - 1, 0);
			snd_parameters();
			break;
		case 27:
            if ((c = term_getchar_nb()) != -1 && c == '[' && (c = term_getchar_nb()) != -1) {
                process_key(c);
                break;
            }
			snd_abort_or_exit();
			break;
		default:
			term_puts("\n[Keyboard]\tUnrecognised key ");
			term_putchar(c);
	}
	return 1;
}

/**
 * Author: Chiel Bruin
 */
void calc_control_vector(int16_t p_offset, int16_t r_offset, int16_t y_offset, int16_t z_offset)
{
	int16_t pitch = safe_offset(axis[1],  p_offset);
	int16_t roll  = safe_offset(axis[0],  r_offset);
	int16_t yaw   = safe_offset(axis[2],  y_offset);

#ifdef JS_DEV
	int16_t lift  = calculate_lift(-axis[3], z_offset);
#else
	int16_t lift  = calculate_lift(INT16_MIN, z_offset);
#endif

	assert(lift >= 0, "Negative lift");
    
    // Check if the throttle is moved in height mode.
    // If so, exit to full_control
    if (incoming_control_mode == height_mode) {
        if (lift != old_lift) {
            snd_control_mode(full_control_mode);
        }
    } else {
        old_lift = lift;
    }
	snd_control_vector(pitch, roll, yaw, lift); // away with ye
	snd_control_vector_gui(p_offset, pitch, r_offset, roll, y_offset, yaw, z_offset,  lift); // away with ye
}
