/**
 * Phosphor renderer/opengl_renderer.cpp
 * OpenGL renderer implementation
 */

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include "phosphor/mesh/cube.h"
#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"
#include "phosphor/shader.h"
#include "phosphor/camera.h"

void OpenGLRenderer::init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::critical("SDL_Init: {}", SDL_GetError());
        return;
    }

    this->window = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_OPENGL);
    if(this->window == nullptr) {
        spdlog::critical("SDL_CreateWindow: {}", SDL_GetError());
        return;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if(this->context == nullptr) {
        spdlog::critical("SDL_GL_CreateContext: {}", SDL_GetError());
        return;
    }

    if(glewInit() != GLEW_OK) {
        spdlog::critical("glewInit failed");
        return;
    }

    spdlog::info("OpenGL version: {}", glGetString(GL_VERSION));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGui_ImplSDL2_InitForOpenGL(this->window, this->context);
    ImGui_ImplOpenGL3_Init("#version 330");
} // OpenGLRenderer::init

void OpenGLRenderer::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
} // OpenGLRenderer::shutdown

void OpenGLRenderer::run() {
    bool running = true;
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, 1366, 768);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(this->window);
    }
} // OpenGLRenderer::run
