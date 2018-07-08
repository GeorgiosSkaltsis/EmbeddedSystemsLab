/**
 * logger.c
 * 
 * implementation of wrapper to store and flush logs
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "logger.h"
#include "in4073.h"
#include <stdlib.h>

#define log_id_size 7

uint16_t log_id = 0;

void logger_init()
{
    address_cur = address_min;
}

void log_message(byte_p buffer, uint8_t size)
{
    if (address_cur + (size + log_id_size + 1) * sizeof(uint8_t) > address_max) {
        return;
    }

    // Prepare ID message
    char id_string[log_id_size];
    sprintf(id_string, "%4d | ", log_id);
    log_id += 1;

    // Write log size to flash
    uint8_t size_with_id = log_id_size + size;
    flash_write_bytes(address_cur, &size_with_id, sizeof(uint8_t));
    address_cur += sizeof(uint8_t);

    // Write id to flash
    flash_write_bytes(address_cur, (uint8_t *) id_string, log_id_size * sizeof(uint8_t));
    address_cur += log_id_size * sizeof(uint8_t);

    // Write message to flash
    flash_write_bytes(address_cur, (uint8_t *) buffer, size * sizeof(uint8_t));
    address_cur += size * sizeof(uint8_t);
}

void flush_logs()
{
    // Send all stored logs to protocol
    uint32_t address_flush = address_min;
    while (address_flush < address_cur) 
    {
        // Read log size from flash
        uint8_t size;
        flash_read_bytes(address_flush, &size, sizeof(uint8_t));
        address_flush += sizeof(uint8_t);

        // Read log message from flash
        uint8_t* data = malloc(size * sizeof(uint8_t));
        flash_read_bytes(address_flush, data, size);
        snd_log((byte_p) data, size);
        free(data);
        address_flush += size * sizeof(uint8_t);

        // Busy wait a bit, or some @@#$! queue SOMEWHERE fills up
        uint32_t wait_time = get_time_us() + 10000;
        while (get_time_us() < wait_time) { }
    }
}

void clear_logs()
{
    flash_chip_erase();
    address_cur = address_min;
}