/**
 * graphs.hpp
 * 
 * Draws a bunch of graphs to the screen
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#include <memory>
#include <vector>

#define default_num_graph_values 50
#define graph_pixel_height 33.f
#define min_motor_value 0.f
#define max_motor_value 500.f
#define min_angle_value -32768.f
#define max_angle_value 32768.f
#define min_bat_value 0.f
#define max_bat_value 9999.f
#define min_temperature_value 0.f
#define max_temperature_value 9999.f
#define min_pressure_value 0.f
#define max_pressure_value 999999.f
#define min_roundtrip_time 0.f
#define max_roundtrip_time 100.f

// Forward declaration
struct protocol_t;
typedef std::shared_ptr<protocol_t> protocol_p;

class graphs_t {
public:
    graphs_t(protocol_p protocol);

    void process_info_data(std::vector<std::string> info_data);
    void process_ping_data(float ping);
    void update();

private:
    void clear_arrays();
    
    int num_graph_values, old_graph_values;
    protocol_p protocol;
    
    std::vector<float> motor_0;
    std::vector<float> motor_1;
    std::vector<float> motor_2;
    std::vector<float> motor_3;
    
    std::vector<float> phi;
    std::vector<float> theta;
    std::vector<float> psi;
    
    std::vector<float> sp;
    std::vector<float> sq;
    std::vector<float> sr;

    std::vector<float> bat_volt;
    std::vector<float> temperature;
    std::vector<float> pressure;
    std::vector<float> ping_roundtrip_time;
};

typedef std::shared_ptr<graphs_t> graphs_p;