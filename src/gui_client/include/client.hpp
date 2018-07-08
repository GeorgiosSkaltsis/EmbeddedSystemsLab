/**
 * client.hpp
 * 
 * Client for the GUI to connect to a server.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#define pc_to_gui_s "/tmp/pc_to_gui"
#define gui_to_pc_s "/tmp/gui_to_pc"

struct client_t {
    int connect();
    int disconnect();
    int write(const char* buffer, int size);
    int write(char c);
    char read();

    int fd_rd, fd_wr;
};