/**
 * frame.h
 * 
 * defines, types, function prototypes for frame used in comm protocol
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef SHARED_FRAMES_H
#define SHARED_FRAMES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "types.h"

#define preamble_str "aaaa"
#define preamble_len 4
#define timestamp_max_offset 10

// Frame header with preamble and payload information
typedef struct frame_hdr_t {
    byte_t preamble[preamble_len];
    byte_t type;
    uint8_t size;
} frame_hdr_t;

// Frame footer with error detection
typedef struct frame_ftr_t {
    byte_t interleaving;
} frame_ftr_t;

// Frame with variable length payload
typedef struct frame_t {
    frame_hdr_t header;
    byte_p payload;
    frame_ftr_t footer;
} frame_t;
typedef frame_t* frame_p;

// Create and return pointer to frame for given payload of given size
frame_p new_frame(byte_p payload, byte_t type, uint8_t size);

// Frame constructor/destructor
void constr_frame(frame_p frame, byte_p payload, byte_t type, uint8_t size);
void destr_frame(frame_p frame);

// Duplicate and return pointer to duplicated frame
frame_p dupl_frame(frame_p frame);

// length of frame in bytes
uint16_t frame_length(frame_p frame);

// Compute interleaving of frame bytes
byte_t frame_to_interleaving(frame_p frame);

// Translate frame to pointer to buffer of bytes
byte_p frame_to_buffer(frame_p frame);

#ifdef __cplusplus
};
#endif

#endif // SHARED_FRAMES_H


