/**
 * imgui_wrapper.hpp
 * 
 * implementation of a wrapper around imgui
 * 
 * M. van de Ruit
 * 
 * May 2018
 */

#include "imgui_wrapper.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

imgui_wrapper_t::imgui_wrapper_t(window_p window)
{
    this->window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui_ImplGlfwGL3_Init(window->window_p, true);
    ImGui::StyleColorsDark();
}

imgui_wrapper_t::~imgui_wrapper_t()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void imgui_wrapper_t::pre_update()
{
    ImGui_ImplGlfwGL3_NewFrame();
}

void imgui_wrapper_t::post_update()
{
    glViewport(0, 0, window->buffer_width, window->buffer_height);
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}