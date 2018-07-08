/**
 * background.hpp
 * 
 * Draws an image to a background
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#pragma once

#include <memory>
#include "GL/glew.h"
#include "window.hpp"

class background_t {
public:
    background_t(window_p window);
    void update();
private:
    window_p window;
    GLuint program;
    GLuint vao;
    GLuint texture;
};

typedef std::shared_ptr<background_t> background_p;