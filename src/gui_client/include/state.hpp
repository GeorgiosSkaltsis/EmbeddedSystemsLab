/**
 * state.hpp
 * 
 * Draws some state info and buttons to the screen
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

#define bat_warning 1100
#define bat_minimal 1050

// Forward declaration
struct protocol_t;
typedef std::shared_ptr<protocol_t> protocol_p;

class state_t {
public:
    state_t(protocol_p protocol);

    void process_info_data(std::vector<std::string> info_data);
    void update();

private:
    void send_mode(control_mode_e mode);

    std::string mode_string;
    int mode;
    int bat;
    int bat_state;
    protocol_p protocol;
};

typedef std::shared_ptr<state_t> state_p;