/**
 * ping_timer.c
 * 
 * Ping message timer, so drone is kept alive.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "ping_timer.h"
#include <time.h>
#include "comm_terminal.h"

clock_t last_message_time;

void ping_timer_init()
{
    last_message_time = clock();
}

void ping_timer_update()
{
    clock_t current = clock();
    clock_t diff = current - last_message_time;
    float diff_ms = 1000.f * (float) diff / CLOCKS_PER_SEC;
    if (diff_ms >= ping_message_time_ms)
    {
        last_message_time = current;
        snd_ping();
    }
}