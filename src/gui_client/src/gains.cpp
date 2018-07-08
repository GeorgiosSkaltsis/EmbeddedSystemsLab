/**
 * gains.hpp
 * 
 * Allows tweaking of gains parameters
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "gains.hpp"
#include "imgui.h"
#include <iostream>
#include "protocol.hpp"
#include <inttypes.h>
#include "misc.hpp"

gains_t::gains_t(protocol_p protocol)
{
    this->protocol = protocol;
}

void gains_t::snd_parameters()
{
    int8_t message[5] = {
        (int8_t) k_y,
        (int8_t) k_r_1,
        (int8_t) k_r_2,
        (int8_t) k_h,
        0
    };
    frame_p frame = new_frame((byte_p) &message, parameter_payload, parameter_size);
    protocol->snd_frame(frame);
    destr_frame(frame);
}

void gains_t::process_info_data(std::vector<std::string> info_data)
{
    k_y = misc::safe_stoi(info_data[14]);
    k_r_1 = misc::safe_stoi(info_data[15]);
    k_r_2 = misc::safe_stoi(info_data[16]);
    k_h = misc::safe_stoi(info_data[17]);
}

void gains_t::update()
{
    ImGui::Begin("Gains");

    ImGui::Text("K_y: %i", k_y);
    ImGui::SameLine();
    if (ImGui::ArrowButton("Increase k_y", ImGuiDir_Up))
    {
        k_y += 1;
        snd_parameters();
    }
    if (k_y > 0)
    {
        ImGui::SameLine();
        if (ImGui::ArrowButton("Decrease k_y", ImGuiDir_Down))
        {
            k_y = std::max(0, k_y - 1);
            snd_parameters();
        }
    }
    
    ImGui::Separator();

    ImGui::Text("K_r_1: %i", k_r_1);
    ImGui::SameLine();
    if (ImGui::ArrowButton("Increase k_r_1", ImGuiDir_Up))
    {
        k_r_1 += 1;
        snd_parameters();
    }
    if (k_r_1 > 0)
    {
        ImGui::SameLine();
        if (ImGui::ArrowButton("Decrease k_r_1", ImGuiDir_Down))
        {
            k_r_1 = std::max(0, k_r_1 - 1);
            snd_parameters();
        }
    }

    ImGui::Separator();

    ImGui::Text("K_r_2: %i", k_r_2);
    ImGui::SameLine();
    if (ImGui::ArrowButton("Increase k_r_2", ImGuiDir_Up))
    {
        k_r_2 += 1;
        snd_parameters();
    }
    if (k_r_2 > 0)
    {
        ImGui::SameLine();
        if (ImGui::ArrowButton("Decrease k_r_2", ImGuiDir_Down))
        {
            k_r_2 = std::max(0, k_r_2 - 1);
            snd_parameters();
        }
    }

    ImGui::Separator();

    ImGui::Text("K_h: %i", k_h);
    ImGui::SameLine();
    if (ImGui::ArrowButton("Increase k_h", ImGuiDir_Up))
    {
        k_h += 1;
        snd_parameters();
    }
    if (k_h > 0)
    {
        ImGui::SameLine();
        if (ImGui::ArrowButton("Decrease k_h", ImGuiDir_Down))
        {
            k_h = std::max(0, k_h - 1);
            snd_parameters();
        }
    }

    ImGui::End();
}
