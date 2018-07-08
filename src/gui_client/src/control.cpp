/**
 * control.cpp
 * 
 * Allows for input control, similar to joystick and keyboard usage.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "control.hpp"
#include "imgui.h"
#include <iostream>
#include "protocol.hpp"
#include <inttypes.h>

control_t::control_t(protocol_p protocol)
{
    this->protocol = protocol;
}

void control_t::snd_key(char k)
{
    
}

void control_t::process_control_offsets(int roll_offset, int pitch_offset, int yaw_offset, int lift_offset)
{
    this->roll_offset = roll_offset;
    this->pitch_offset = pitch_offset;
    this->yaw_offset = yaw_offset;
    this->lift_offset = lift_offset;
}

void control_t::process_control_data(int roll, int pitch, int yaw, int lift)
{
    this->roll = roll;
    this->pitch = pitch;
    this->yaw = yaw;
    this->lift = lift;
}

void control_t::update()
{
    ImGui::Begin("Control");
    if (ImGui::TreeNodeEx("Processed", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Roll: %i", roll);
        ImGui::Text("Pitch: %i", pitch);
        ImGui::Text("Yaw: %i", yaw);
        ImGui::Text("Lift: %i", lift);
        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::TreeNodeEx("Unprocessed", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Roll: %i", roll_offset);
        ImGui::Text("Pitch: %i", pitch_offset);
        ImGui::Text("Yaw: %i", yaw_offset);
        ImGui::Text("Lift: %i", lift_offset);
        ImGui::TreePop();
    }
    ImGui::End();
}