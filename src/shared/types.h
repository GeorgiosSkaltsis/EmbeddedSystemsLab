/**
 * types.h
 * 
 * defines, types for shared headers.
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#ifndef SHARED_TYPES_H
#define SHARED_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

typedef char byte_t;
typedef byte_t* byte_p;

// Payload type
typedef enum payload_type_e {
    log_payload = 0,
    info_payload = 1,
    control_vector_payload = 2,
    control_mode_payload = 3,
    abort_or_exit_payload = 4,
    parameter_payload = 5,
    battery_low_payload = 6,
    key_payload = 7,
    control_vector_gui_payload = 8,
    ping_payload = 9,
} payload_type_e;

// Payload size
typedef enum payload_size_e {
    // logs have variable length payloads, so it's not included
    info_size = 119,
    control_vector_size = 8,
    control_mode_size = 1,
    abort_or_exit_size = 1,
    battery_low_size = 1,
    parameter_size = 5,
    key_size = 1,
    control_vector_gui_size = 16,
    ping_payload_size = 1,
} payload_size_e;

// Find size for type
uint8_t type_to_size(payload_type_e t);

// Control modes
typedef enum control_mode_e {
    // Required modes
    safe_mode = 0,
    panic_mode = 1, 
    manual_mode = 2,
    calibrate_mode = 3,
    yaw_control_mode = 4,
    full_control_mode = 5,

    // Optional modes
    raw_mode = 6,
    height_mode = 7,
    // wireless_mode = 8,
} control_mode_e;

#ifdef __cplusplus
};
#endif

#endif // SHARED_TYPES_H
