/**
 * protocol.hpp
 * 
 * Cpp version of protocol so GUI can use it
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#include <memory>
#include "client.hpp"
#include "frame.h"

// Forward declaration
struct gui_t;
typedef std::shared_ptr<gui_t> gui_p;

struct protocol_t {
    protocol_t();
    ~protocol_t();

    void update_rcv();
    void snd_frame(frame_p frame);

    client_t client;
    gui_p gui;
};

typedef std::shared_ptr<protocol_t> protocol_p;