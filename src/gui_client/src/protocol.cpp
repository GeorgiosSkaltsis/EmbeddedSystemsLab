/**
 * protocol.cpp
 * 
 * Cpp version of protocol so GUI can use it
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include <iostream>
#include "comm_protocol.h"
#include "protocol.hpp"
#include "gui.hpp"

protocol_t::protocol_t()
{
    if (client.connect() < 0)
    {
        std::cout << "Could not connect to server" << std::endl;
        exit(EXIT_FAILURE);
    }
    client.write("Hello there", 11);
}

protocol_t::~protocol_t()
{
    client.disconnect();
}

void protocol_t::update_rcv()
{
    // Store incoming byte in receive queue
    char c;
    while ((c = client.read()) != -1) 
    {
        byte_enqueue(&rcv_queue, c);
    }

    // Empty queue looking for frames
    while (rcv_queue.count > 0)
    {
        frame_p frame = rcv_frame();
        if (frame != NULL) 
        {
            switch(frame->header.type) 
            {
                case info_payload:
                    gui->process_info_log(frame->payload);
                    break;
                case control_vector_gui_payload:
                    gui->process_control_vector(frame->payload);
                    break;
                case ping_payload:
                    gui->process_ping_time(frame->payload);
                    break;
                default:
                    // ignore
                    break;
            }
            destr_frame(frame);
        }
    }
}


void protocol_t::snd_frame(frame_p frame)
{
    for (uint8_t i = 0; i < sizeof(frame_hdr_t); i++) {
        client.write(((byte_p) &frame->header)[i]);
    }
    for (uint8_t i = 0; i < frame->header.size; i++) {
        client.write(frame->payload[i]);
    }
    for (uint8_t i = 0; i < sizeof(frame_ftr_t); i++) {
        client.write(((byte_p) &frame->footer)[i]);
    }
    client.write('x');
}
