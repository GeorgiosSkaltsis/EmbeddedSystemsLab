/**
 * byte_queue.h
 * 
 * defines, types, function prototypes for incoming/outgoing byte queue
 * 
 * M. van de Ruit
 *
 * May 2018
 */

#ifndef SHARED_BYTE_QUEUE_H
#define SHARED_BYTE_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "types.h"

#define byte_queue_size 512

// Frame queue type
typedef struct byte_queue_t {
    byte_t bytes[byte_queue_size];
    uint16_t first, last, count;
} byte_queue_t;
typedef byte_queue_t* byte_queue_p;

// Create and use a frame queue
void constr_byte_queue(byte_queue_p queue);
void byte_enqueue(byte_queue_p queue, byte_t byte);
byte_t byte_dequeue(byte_queue_p queue);

#ifdef __cplusplus
};
#endif

#endif // BYTE_QUEUE_H