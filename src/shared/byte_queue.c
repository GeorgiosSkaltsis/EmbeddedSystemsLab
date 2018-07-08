/**
 * byte_queue.c
 * 
 * implementation of incoming/outgoing byte queue
 * 
 * M. van de Ruit
 *
 * May 2018
 */

#include "byte_queue.h"

void constr_byte_queue(byte_queue_p q)
{
    q->first = 0;
    q->last = byte_queue_size - 1;
    q->count = 0;
}

void byte_enqueue(byte_queue_p q, byte_t b)
{
    q->last = (q->last + 1) % byte_queue_size;
    q->bytes[q->last] = b;
    q->count += 1;
}

byte_t byte_dequeue(byte_queue_p q)
{
    byte_t b = q->bytes[q->first];
    q->first = (q->first + 1) % byte_queue_size;
    q->count -= 1;
    return b;
}