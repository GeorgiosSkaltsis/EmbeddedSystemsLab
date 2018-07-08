/**
 * ping_timer.h
 * 
 * Ping message timer, so drone is kept alive.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#define ping_message_time_ms 100.0f // 50 ms intervals for ping

void ping_timer_init();
void ping_timer_update();