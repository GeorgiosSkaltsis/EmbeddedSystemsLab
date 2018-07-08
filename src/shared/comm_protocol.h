/**
 * comm_protocol.h
 * 
 * defines, types, function prototypes for communication protocol
 * 
 * M. van de Ruit
 *
 * May 2018
 */

#ifndef SHARED_COMM_PROTOCOL_H
#define SHARED_COMM_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "types.h"
#include "byte_queue.h"
#include "frame.h"

// Queues for sending/receiving bytes
byte_queue_t rcv_queue;

// Protocol usage
void comm_protocol_init();
void comm_protocol_update_rcv();
frame_p rcv_frame();

#ifdef __cplusplus
};
#endif

#endif // SHARED_COMM_PROTOCOL_H
