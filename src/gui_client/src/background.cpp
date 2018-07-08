/**
 * background.cpp
 * 
 * Draws an image to a background
 * 
 * M. van de Ruit
 * 
 * June 2018
 */

#include "background.hpp"
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void gl_check_shader_compile(GLuint shader)
{
    GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> shaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
		std::cerr << &shaderError[0] << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}

void gl_check_program_compile(GLuint program)
{
    GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}

background_t::background_t(window_p window)
{
    this->window = window;

    const std::string vertex_string =
        "#version 330 core\n" // old gl version
        "layout(location = 0) in vec4 v_pos;\n"
        "out vec2 t_pos;\n"
        "void main() {\n"
            "t_pos = 0.5 + 0.5 * v_pos.xy;\n"
            "gl_Position = v_pos;\n"
        "}";
    const std::string fragment_string =
        "#version 330 core\n" // old gl version
        "uniform sampler2D tex;\n"
        "uniform float fov_y;\n"
        "in vec2 t_pos;\n"
        "out vec3 f_col;\n"
        "void main() {\n"
            "f_col = vec3(texture(tex, t_pos * vec2(1.0, fov_y)));\n"
        "}";
    const char* vertex_c_string = vertex_string.c_str();
    const char* fragment_c_string = fragment_string.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_c_string, NULL);
	glCompileShader(vertex_shader);
    gl_check_shader_compile(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_c_string, nullptr);
	glCompileShader(fragment_shader);
    gl_check_shader_compile(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
	glLinkProgram(program);
    gl_check_program_compile(program);

    glDetachShader(program, vertex_shader);
    glDeleteShader(vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(fragment_shader);

    // Load texture
	stbi_set_flip_vertically_on_load(true);
	std::string filename = std::string(DATA_DIR) + "/background.jpg";
    int x,y,n;
    auto data = stbi_load(filename.c_str(), &x, &y, &n, 0);
    if (data == nullptr) {
        std::cerr << "Error, could not load " << filename << ". File may not exist or be supported." << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // Prep texture for rendering
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Don't forget to dump loaded data
    stbi_image_free(data);

    // Create a screen quad, useful for drawing buffers to sreeen
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLfloat quad[] = {
        -1.f, 1.f,
        -1.f, -1.f,
        1.f, 1.f,
        1.f, -1.f
    };
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
}

void background_t::update()
{
    float fov_y = (float) window->buffer_height / (float) window->buffer_width;

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glUniform1f(glGetUniformLocation(program, "fov_y"), fov_y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
}