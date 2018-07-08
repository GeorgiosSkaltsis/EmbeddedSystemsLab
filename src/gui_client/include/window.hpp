/**
 * window.hpp
 * 
 * declaration of window class that wraps a GLFWindow
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "GLFW/glfw3.h"

struct window_t {
    window_t(const int width = 1024, const int height  = 768, const std::string title = "window");
    ~window_t();

    void update();
    void make_current_context();
	void close();
	bool should_close();

    int width, height;
    int buffer_width, buffer_height;
    std::string title;
    GLFWwindow* window_p;
};

typedef std::shared_ptr<window_t> window_p;
