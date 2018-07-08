/**
 * logger.h
 * 
 * declaration of wrapper to store and flush logs
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef DRONE_LOGGER_H
#define DRONE_LOGGER_H

#include <inttypes.h>
#include "../shared/types.h"

#define address_min 0x000000
#define address_max 0x01FFFF

uint32_t address_cur;

void logger_init();
void log_message(byte_p buffer, uint8_t size);
void flush_logs();
void clear_logs();

#endif // DRONE_LOGGER_H