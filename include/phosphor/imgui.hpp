#pragma once

/**
 * Phosphor include/phosphor/imgui.hpp
 * ImGui utilities for Phosphor.
 */

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL2/SDL.h>

//TODO: create functions for objectifying frames and components (ImGuiFrame class, node subclasses?)

inline void imgui_init(SDL_Window* window, void* backend_context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, backend_context);
    ImGui_ImplOpenGL3_Init("#version 450");
}

inline void imgui_shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

inline void imgui_newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

inline void imgui_render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void imgui_processEvent(const SDL_Event* event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}