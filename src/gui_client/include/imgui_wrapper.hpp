/**
 * imgui_wrapper.hpp
 * 
 * declaration of a wrapper around imgui
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#pragma once
#include "window.hpp"
#include <memory>

class imgui_wrapper_t {
public:
    imgui_wrapper_t(window_p window);
    ~imgui_wrapper_t();

    void pre_update();
    void post_update();

private:
    window_p window;
};

typedef std::shared_ptr<imgui_wrapper_t> imgui_wrapper_p;