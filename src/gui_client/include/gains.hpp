/**
 * gains.hpp
 * 
 * Allows tweaking of gains parameters
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

// Forward declaration
struct protocol_t;
typedef std::shared_ptr<protocol_t> protocol_p;

class gains_t {
public:
    gains_t(protocol_p protocol);
    
    void process_info_data(std::vector<std::string> info_data);
    void update();
    
private:
    void snd_parameters();

    protocol_p protocol;
    int k_y, k_r_1, k_r_2, k_h;
};

typedef std::shared_ptr<gains_t> gains_p;