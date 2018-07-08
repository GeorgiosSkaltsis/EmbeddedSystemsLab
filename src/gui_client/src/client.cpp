/**
 * client.cpp
 * 
 * Client for the GUI to connect to a server.
 * 
 * M. van de Ruit
 * 
 * June 2018
*/

#include "client.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int client_t::connect()
{
    // Create pipes, this fails if they exist, no worries
    if (mknod(pc_to_gui_s, S_IFIFO | 0660, 0) < 0)
    {
        perror("mknod pc_to_gui");
    }
    if (mknod(gui_to_pc_s, S_IFIFO | 0660, 0) < 0)
    {
        perror("mknod gui_to_pc");
    }

    // Open pipes
    if ((fd_wr = open(gui_to_pc_s, O_RDWR | O_NONBLOCK)) < 0)
    {
        perror("open gui_to_pc");
    }
    if ((fd_rd = open(pc_to_gui_s, O_RDWR | O_NONBLOCK)) < 0)
    {
        perror("open pc_to_gui");
    }

    // Clear out pipes
    char buffer[2];
    buffer[1] = '\0';
    while (::read(fd_rd, buffer, 1) > 0) { }
    while (::read(fd_wr, buffer, 1) > 0) { }

    return 0;
}

int client_t::disconnect()
{
    close(fd_wr);
    close(fd_rd);
    return 0;
}

char client_t::read()
{
    char buffer[2];
    buffer[1] = '\0';

    if (::read(fd_rd, buffer, 1) > 0)
    {
        return buffer[0];
    }
    else
    {
        return -1;
    }
}

int client_t::write(const char* buffer, int size)
{
    if (::write(fd_wr, buffer, size) < 0)
    {
        // perror("write");
        return -1;
    }
    return 0;
}

int client_t::write(char c)
{
    char buffer[2];
    buffer[0] = c;
    buffer[1] = '\0';
    return write(buffer, 1);
}