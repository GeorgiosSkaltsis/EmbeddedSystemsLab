/**
 * frame.c
 * 
 * implementation of frame used in comm protocol.
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "frame.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

frame_p new_frame(byte_p payload, byte_t type, uint8_t size)
{
    frame_p frame = malloc(sizeof(frame_t));
    constr_frame(frame, payload, type, size);
    return frame;
}

void constr_frame(frame_p frame, byte_p payload, byte_t type, uint8_t size)
{
    // Set frame header
    memcpy(frame->header.preamble, preamble_str, preamble_len);
    frame->header.type = type;
    frame->header.size = size;

    // Copy payload over if needed
    if (size > 0) {
        frame->payload = malloc(size);
        memcpy(frame->payload, payload, size);
    } else {
        frame->payload = NULL;
    }

    // Set frame footer
    frame->footer.interleaving = frame_to_interleaving(frame);
}

void destr_frame(frame_p frame) 
{
    if (frame->payload != NULL) {
        free(frame->payload);
    }
    free(frame);
}

uint16_t frame_length(frame_p frame)
{
    return (uint16_t) 
        sizeof(frame_hdr_t) + 
        frame->header.size +
        sizeof(frame_ftr_t);
}

frame_p dupl_frame(frame_p frame)
 {
     frame_p duplicate = malloc(sizeof(frame_t));
     memcpy(&duplicate->header, &frame->header, sizeof(frame_hdr_t));
     if (frame->header.size > 0) {
        duplicate->payload = malloc(frame->header.size);
        memcpy(duplicate->payload, frame->payload, frame->header.size);
     } else {
         duplicate->payload = NULL;
     }
     memcpy(&duplicate->footer, &frame->footer, sizeof(frame_ftr_t));
     return duplicate;
 }

byte_t frame_to_interleaving(frame_p frame)
{
    byte_t interleaving = ((byte_p) &frame->header)[0];
    for (uint8_t i = 1; i < sizeof(frame_hdr_t); i++) {
        interleaving ^= ((byte_p) &frame->header)[i];
    }
    for (uint8_t i = 0; i < frame->header.size; i++) {
        interleaving ^= frame->payload[i];
    }
    // interleaving itself is not included in the interleaving, so we skip it
    return interleaving;
}

byte_p frame_to_buffer(frame_p frame) 
{
    byte_p bytes = malloc(sizeof(frame_hdr_t) + frame->header.size + sizeof(frame_ftr_t));
    memcpy(bytes, &frame->header, sizeof(frame_hdr_t));
    if (frame->header.size > 0) {
        memcpy(bytes + sizeof(frame_hdr_t), frame->payload, frame->header.size);
    }
    memcpy(bytes + sizeof(frame_hdr_t) + frame->header.size, &frame->footer, sizeof(frame_ftr_t));
    return bytes;
}

