/**
 * main.cpp
 * 
 * Starting point and core loop for GUI
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include <iostream>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_wrapper.hpp"
#include "background.hpp"
#include "protocol.hpp"
#include "gui.hpp"

int main(int argc, char** argv)
{
    // Establish connection with the server before we do anything else
    auto protocol = std::make_shared<protocol_t>();
    auto gui = std::make_shared<gui_t>(protocol);
    protocol->gui = gui;

    // Setup rendering target for GUI
    int glfw_init_code = glfwInit();
    if (!glfw_init_code) {
        std::cerr << "GLFW init error: " << glfw_init_code << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

// #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #endif    

    // Create window for rendering context
    auto window_p = std::make_shared<window_t>(1024, 768, "Mission Control");

    // GL didn't break
    GLenum gl_err_code = glGetError();
    if (gl_err_code != GL_NO_ERROR) {
        std::cerr << "GL init error after window/context creation: " << gl_err_code << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // GLEW didn't break
    GLenum glew_init_code = glewInit();
    if (glew_init_code != GLEW_OK) {
        std::cerr << "GLEW init error: " << glew_init_code << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // Initialize IMGUI
    auto background = std::make_shared<background_t>(window_p);
    auto imgui_p = std::make_shared<imgui_wrapper_t>(window_p);

    // Core update loop
    while (!window_p->should_close()) {
        // Skip loop if window is minimized or too small
        if (window_p->buffer_width == 0 || window_p->buffer_height == 0) {
            window_p->update();
            continue;
        }
        
        // Prep for rendering
        glViewport(0, 0, window_p->buffer_width, window_p->buffer_height);
        glClear(GL_COLOR_BUFFER_BIT);
        imgui_p->pre_update();

        // Update data
        protocol->update_rcv();
        gui->update();
        
        // Render
        background->update();
        imgui_p->post_update();
        window_p->update();
    }
        
    exit(EXIT_SUCCESS);
}