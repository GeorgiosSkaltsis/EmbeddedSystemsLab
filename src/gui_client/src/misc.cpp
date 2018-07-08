/**
 * misc.cpp
 * 
 * A number of miscellaneous helper functions.
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "misc.hpp"
#include <iostream>

int misc::safe_stoi(std::string in)
{
    int out;
    try {
        out = std::stoi(in);
    } catch (...) {
        out = 0;
        std::cout << "(std::stoi) suspecting corrupt data: " << out << std::endl;
    }
    return out;
}