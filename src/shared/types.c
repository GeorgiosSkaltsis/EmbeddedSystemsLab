/**
 * types.c
 * 
 * implementation for functions of shared types
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "types.h"

uint8_t type_to_size(payload_type_e t)
{
    switch (t) {
        case info_payload:
            return info_size;
        case control_vector_payload:
            return control_vector_size;
        case control_mode_payload:
            return control_mode_size;
        case abort_or_exit_payload:
            return abort_or_exit_size;
        case parameter_payload:
            return parameter_size;
        case battery_low_payload:
            return battery_low_size;
        default:
            return 0;
    }
}