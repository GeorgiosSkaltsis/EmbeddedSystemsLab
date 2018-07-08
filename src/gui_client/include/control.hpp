/**
 * control.hpp
 * 
 * Allows for input control, similar to joystick and keyboard usage.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include "types.h"

// Forward declaration
struct protocol_t;
typedef std::shared_ptr<protocol_t> protocol_p;

class control_t {
public:
    control_t(protocol_p protocol);

    void process_control_offsets(int roll_offset, int pitch_offset, int yaw_offset, int lift_offset);
    void process_control_data(int roll, int pitch, int yaw, int lift);
    void update();

private:
    void snd_key(char k);
    
    protocol_p protocol;
    int roll, pitch, yaw, lift;
    int roll_offset, pitch_offset, yaw_offset, lift_offset;
};

typedef std::shared_ptr<control_t> control_p;