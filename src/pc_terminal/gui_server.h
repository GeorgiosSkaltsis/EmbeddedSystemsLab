/**
 * gui_server.h
 * 
 * Server for the GUI to connect to.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#ifndef GUI_SERVER_H
#define GUI_SERVER_H

#include <stdbool.h>
#include <inttypes.h>
#include "../shared/types.h"
#include "../shared/values.h"
#include "../shared/byte_queue.h"
#include "../shared/frame.h"

#define pc_to_gui_s "/tmp/pc_to_gui"
#define gui_to_pc_s "/tmp/gui_to_pc"

int fd_rd, fd_wr;

int server_init();
int server_disconnect();
int server_write(char* buffer, int size);
int server_write_c(char c);
char server_read();

byte_queue_t gui_rcv_queue;
void comm_gui_init();
void comm_gui_update_rcv();
void comm_gui_snd_frame(frame_p frame);
frame_p comm_gui_rcv_frame();

void rcv_control_mode(byte_p mode);
void rcv_parameters(byte_p parameters);
void rcv_key(byte_p key);
void snd_control_vector_gui(int16_t p_offset, int16_t pitch, int16_t r_offset, int16_t roll, int16_t y_offset, int16_t yaw, int16_t z_offset, int16_t lift);

#endif // GUI_SERVER_H