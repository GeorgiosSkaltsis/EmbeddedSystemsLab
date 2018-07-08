/**
 * gui.cpp
 * 
 * The main gui wrapping a number of components
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "gui.hpp"
#include <string>
#include <vector>
#include <sstream>
#include "protocol.hpp"
#include <time.h>
#include <iostream>

gui_t::gui_t(protocol_p protocol)
{
    graphs = std::make_shared<graphs_t>(protocol);
    state = std::make_shared<state_t>(protocol);
    gains = std::make_shared<gains_t>(protocol);
    control = std::make_shared<control_t>(protocol);
}

void gui_t::update()
{
    graphs->update();   
    state->update();
    gains->update();
    control->update();
}

void gui_t::process_info_log(char* info_log)
{
    // We are going to split the info log on delimiters and remove all spaces
    std::string segment;
    std::vector<std::string> segments;

    // Split on first delimiter
    std::stringstream outer_stream(info_log);
    while(std::getline(outer_stream, segment, '|'))
    {
        // Split on second delimiter
        std::stringstream inner_stream(segment);
        while (std::getline(inner_stream, segment, ' '))
        {
            // Not just empty noise
            if(segment.find_first_not_of(' ') != std::string::npos)
            {
                segments.push_back(segment);
            }
        }
    }

    graphs->process_info_data(segments);
    state->process_info_data(segments);
    gains->process_info_data(segments);
}

void gui_t::process_ping_time(char* bytes)
{
    clock_t time_diff = * (clock_t *) bytes;
    float time_ms = (1000.f * (float) time_diff) / CLOCKS_PER_SEC;
    graphs->process_ping_data(time_ms);
}

void gui_t::process_control_vector(char* bytes)
{
    int16_t* message = (int16_t *) bytes;
    control->process_control_offsets(message[0], message[2], message[4], message[6]);
    control->process_control_data(message[1], message[3], message[5], message[7]);
}