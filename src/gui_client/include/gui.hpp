/**
 * gui.hpp
 * 
 * The main gui wrapping a number of components
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#include <memory>
#include "graphs.hpp"
#include "state.hpp"
#include "gains.hpp"
#include "control.hpp"

// Forward declaration
struct protocol_t;
typedef std::shared_ptr<protocol_t> protocol_p;

struct gui_t {
    gui_t(protocol_p protocol);

    void update();
    void process_info_log(char* string);
    void process_ping_time(char* bytes);
    void process_control_vector(char* bytes);

    graphs_p graphs;
    state_p state;
    gains_p gains;
    control_p control;
};

typedef std::shared_ptr<gui_t> gui_p;