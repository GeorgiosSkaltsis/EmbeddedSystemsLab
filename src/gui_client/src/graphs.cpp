/**
 * graphs.cpp
 * 
 * Draws a bunch of graphs to the screen
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "graphs.hpp"
#include "imgui.h"
#include "types.h"
#include <iostream>
#include "protocol.hpp"
#include "misc.hpp"

graphs_t::graphs_t(protocol_p protocol)
{
    this->protocol = protocol;
    num_graph_values = default_num_graph_values;
    clear_arrays();
}

void graphs_t::process_info_data(std::vector<std::string> info_data)
{
    // New motor values
    motor_0.pop_back();
    motor_1.pop_back();
    motor_2.pop_back();
    motor_3.pop_back();
    motor_0.insert(motor_0.begin(), (float) misc::safe_stoi(info_data[1]));
    motor_1.insert(motor_1.begin(), (float) misc::safe_stoi(info_data[2]));
    motor_2.insert(motor_2.begin(), (float) misc::safe_stoi(info_data[3]));
    motor_3.insert(motor_3.begin(), (float) misc::safe_stoi(info_data[4]));

    // New angle values
    phi.pop_back();
    theta.pop_back();
    psi.pop_back();
    phi.insert(phi.begin(), (float) misc::safe_stoi(info_data[5]));
    theta.insert(theta.begin(), (float) misc::safe_stoi(info_data[6]));
    psi.insert(psi.begin(), (float) misc::safe_stoi(info_data[7]));

    // New accelerometer values
    sp.pop_back();
    sq.pop_back();
    sr.pop_back();
    sp.insert(sp.begin(), (float) misc::safe_stoi(info_data[8]));
    sq.insert(sq.begin(), (float) misc::safe_stoi(info_data[9]));
    sr.insert(sr.begin(), (float) misc::safe_stoi(info_data[10]));

    // New misc values
    bat_volt.pop_back();
    temperature.pop_back();
    pressure.pop_back();
    bat_volt.insert(bat_volt.begin(), (float) misc::safe_stoi(info_data[11]));
    temperature.insert(temperature.begin(), (float) misc::safe_stoi(info_data[12]));
    pressure.insert(pressure.begin(), (float) misc::safe_stoi(info_data[13]));
}

void graphs_t::process_ping_data(float ping)
{
    ping_roundtrip_time.pop_back();
    ping_roundtrip_time.insert(ping_roundtrip_time.begin(), ping);
}

void graphs_t::clear_arrays()
{
    motor_0 = std::vector<float>(num_graph_values, 0.f);
    motor_1 = std::vector<float>(num_graph_values, 0.f);
    motor_2 = std::vector<float>(num_graph_values, 0.f);
    motor_3 = std::vector<float>(num_graph_values, 0.f);

    phi = std::vector<float>(num_graph_values, 0.f);
    theta = std::vector<float>(num_graph_values, 0.f);
    psi = std::vector<float>(num_graph_values, 0.f);

    sp = std::vector<float>(num_graph_values, 0.f);
    sq = std::vector<float>(num_graph_values, 0.f);
    sr = std::vector<float>(num_graph_values, 0.f);

    bat_volt = std::vector<float>(num_graph_values, 0.f);
    temperature = std::vector<float>(num_graph_values, 0.f);
    pressure = std::vector<float>(num_graph_values, 0.f);
    ping_roundtrip_time = std::vector<float>(num_graph_values, 0.f);
}

void graphs_t::update()
{
    // Regen graph values
    if (old_graph_values != num_graph_values)
    {
        old_graph_values = num_graph_values;
        clear_arrays();
    }

    ImVec2 graph_dim(0.f, graph_pixel_height);

    ImGui::Begin("History");

    if (ImGui::TreeNodeEx("Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderInt("Plot points", &num_graph_values, 10, 500);
        if (ImGui::Button("Clear data"))
        {
            clear_arrays();
        }
        ImGui::TreePop();
    }
    ImGui::Separator();

    // Plot motors
    if (ImGui::TreeNodeEx("Motors", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PlotLines("Motor 0", motor_0.data(), motor_0.size(), 0, std::to_string((int) motor_0[0]).c_str(), min_motor_value, max_motor_value, graph_dim);
        ImGui::PlotLines("Motor 1", motor_1.data(), motor_1.size(), 0, std::to_string((int) motor_1[0]).c_str(), min_motor_value, max_motor_value, graph_dim);
        ImGui::PlotLines("Motor 2", motor_2.data(), motor_2.size(), 0, std::to_string((int) motor_2[0]).c_str(), min_motor_value, max_motor_value, graph_dim);
        ImGui::PlotLines("Motor 3", motor_3.data(), motor_3.size(), 0, std::to_string((int) motor_3[0]).c_str(), min_motor_value, max_motor_value, graph_dim);
        ImGui::TreePop();
    }
    ImGui::Separator();

    // Plot phi/theta/psi
    if (ImGui::TreeNodeEx("Angles", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PlotLines("Phi", phi.data(), phi.size(), 0, std::to_string((int) phi[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::PlotLines("Theta", theta.data(), theta.size(), 0, std::to_string((int) theta[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::PlotLines("Psi", psi.data(), psi.size(), 0, std::to_string((int) psi[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::TreePop();
    }
    ImGui::Separator();

    // Plot sp/sq/sr
    if (ImGui::TreeNodeEx("Accelerometers", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PlotLines("sp", sp.data(), sp.size(), 0, std::to_string((int) sp[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::PlotLines("sq", sq.data(), sq.size(), 0, std::to_string((int) sq[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::PlotLines("sr", sr.data(), sr.size(), 0, std::to_string((int) sr[0]).c_str(), min_angle_value, max_angle_value, graph_dim);
        ImGui::TreePop();
    }
    ImGui::Separator();

    // Plot misc values
    if (ImGui::TreeNodeEx("Miscellaneous", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PlotLines("Ping roundtrip time", ping_roundtrip_time.data(), ping_roundtrip_time.size(), 0, std::to_string(ping_roundtrip_time[0]).c_str(), min_roundtrip_time, max_roundtrip_time, graph_dim);
        ImGui::PlotLines("Battery voltage", bat_volt.data(), bat_volt.size(), 0, std::to_string((int) bat_volt[0]).c_str(), min_bat_value, max_bat_value, graph_dim);
        ImGui::PlotLines("Temperature", temperature.data(), temperature.size(), 0, std::to_string((int) temperature[0]).c_str(), min_temperature_value, max_temperature_value, graph_dim);
        ImGui::PlotLines("Pressure", pressure.data(), pressure.size(), 0, std::to_string((int) pressure[0]).c_str(), min_pressure_value, max_pressure_value, graph_dim);
        ImGui::TreePop();
    }
    ImGui::End();
}
