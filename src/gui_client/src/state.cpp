/**
 * state.cpp
 * 
 * Draws some state info and buttons to the screen
 * 
 * M. van de Ruit
 * Edited by C. Bruin
 * 
 * June 2018
 */

#include "state.hpp"
#include "imgui.h"
#include <iostream>
#include "protocol.hpp"
#include "misc.hpp"

state_t::state_t(protocol_p protocol)
{
    this->protocol = protocol;
    mode = -1;
    bat = 0;
    bat_state = -1;
}

void state_t::process_info_data(std::vector<std::string> info_data)
{
    mode = misc::safe_stoi(info_data[18]);
    switch(mode) {
        case safe_mode:
            mode_string = "Safe mode";
            break;
        case panic_mode:
            mode_string = "Panic mode";
            break;
        case manual_mode:
            mode_string = "Manual mode";
            break;
        case calibrate_mode:
            mode_string = "Calibrate mode";
            break;
        case yaw_control_mode:
            mode_string = "Yaw control mode";
            break;
        case full_control_mode:
            mode_string = "Full control mode";
            break;
        case raw_mode:
            mode_string = "Raw mode";
            break;
        case height_mode:
            mode_string = "Height mode";
            break;
        default:
            mode_string = "Unknown";
            break;
    }

    bat = misc::safe_stoi(info_data[11]);
    if (bat < bat_minimal) 
    {
        bat_state = 2;
    }
    else if (bat < bat_warning)
    {
        bat_state = 1;
    }
    else
    {
        bat_state = 0;
    }
}

void state_t::send_mode(control_mode_e mode)
{
    frame_p frame = new_frame((byte_p) &mode, control_mode_payload, control_mode_size);
    protocol->snd_frame(frame);
    destr_frame(frame);
}

void state_t::update()
{
    ImGui::Begin("State");
    ImGui::Text("Current mode: %s", mode_string.c_str());

    ImGui::Separator();

    if (ImGui::Button("Safe mode"))
    {
        send_mode(safe_mode);
    }
    ImGui::SameLine();
    if (ImGui::Button("Panic mode"))
    {
        send_mode(panic_mode);
    }

    if (mode == height_mode)
    {
        ImGui::SameLine();
        if (ImGui::Button("Full control mode"))
        {
            send_mode(full_control_mode);
        }
    }
    
    if (mode == full_control_mode)
    {
        ImGui::SameLine();
        if (ImGui::Button("Height control mode"))
        {
            send_mode(height_mode);
        }
    }
    
    if (mode == safe_mode)
    {
        if (ImGui::Button("Manual mode"))
        {
            send_mode(manual_mode);
        }
        ImGui::SameLine();
        if (ImGui::Button("Calibrate mode"))
        {
            send_mode(calibrate_mode);
        }

        if (ImGui::Button("Yaw control mode"))
        {
            send_mode(yaw_control_mode);
        }
        ImGui::SameLine();
        if (ImGui::Button("Full control mode"))
        {
            send_mode(full_control_mode);
        }
        ImGui::SameLine();
        if (ImGui::Button("Raw mode"))
        {
            send_mode(raw_mode);
        }
        ImGui::SameLine();
        if (ImGui::Button("Height mode"))
        {
            send_mode(height_mode);
        }
    }
    
    ImGui::Separator();

    switch (bat_state)
    {
        case -1:
            ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Battery state: unknown");
            break;
        case 0:
            ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Battery state: normal");
            break;
        case 1:
            ImGui::TextColored(ImVec4(1.f, 0.5f, 0.f, 1.f), "Battery state: GETTING LOW");
            break;
        case 2:
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Battery state: TOO LOW");
            break;
    }
    ImGui::Text("Battery voltage: %i", bat);

    ImGui::End();
}
