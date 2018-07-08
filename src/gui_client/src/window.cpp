/**
 * window.cpp
 * 
 * implementation of window class that wraps a GLFWindow
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "window.hpp"

window_t::window_t(const int width, const int height, const std::string title) 
{
    this->width = width;
    this->height = height;
    this->title = title;
    
    window_p = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_p) {
		std::cerr << "GLFW error creating window: " << title << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    glfwSwapInterval(1); // This should enable vsync
	glfwGetFramebufferSize(window_p, &this->buffer_width, &this->buffer_height);
	glfwGetWindowSize(window_p, &this->width, &this->height);
    glfwMakeContextCurrent(window_p);
}

window_t::~window_t() 
{
	//glfwDestroyWindow(window_p.get());
}

void window_t::update()
{
    glfwPollEvents();
	glfwGetFramebufferSize(window_p, &buffer_width, &buffer_height);
	glfwGetWindowSize(window_p, &width, &height);
	glfwSetWindowTitle(window_p, title.c_str());
    glfwSwapBuffers(window_p);
}

void window_t::make_current_context()
{
    glfwMakeContextCurrent(window_p);
}

void window_t::close()
{
	glfwDestroyWindow(window_p);
}

bool window_t::should_close()
{
    return glfwWindowShouldClose(window_p);
}