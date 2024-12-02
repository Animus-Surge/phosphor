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

#include "phosphor/mesh/axis_widget.h"
#include "phosphor/mesh/cube.h"
#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"
#include "phosphor/shader.h"
#include "phosphor/camera.h"

bool running = true;
Camera* camera;
Shader* shader;

void OpenGLRenderer::init() {
    spdlog::info("Creating OpenGL Renderer...");
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::critical("SDL_Init: {}", fmt::ptr(SDL_GetError()));
        return;
    }

    this->window = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_OPENGL);
    if(this->window == nullptr) {
        spdlog::critical("SDL_CreateWindow: {}", fmt::ptr(SDL_GetError()));
        return;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if(this->context == nullptr) {
        spdlog::critical("SDL_GL_CreateContext: {}", fmt::ptr(SDL_GetError()));
        return;
    }

    if(glewInit() != GLEW_OK) {
        spdlog::critical("glewInit failed");
        return;
    }

    std::string gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::string gl_vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    spdlog::info("OpenGL info [version: {}, vendor: {}]", gl_version, gl_vendor);

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

void OpenGLRenderer::sigterm() {
    running = false;
}

void OpenGLRenderer::run() {
    //TODO: add a debug reset keybind
    //TODO: event controller
    SDL_Event event;

    bool mouse_captured = false;

    AxisWidget* axis_widget = new AxisWidget();
    Cube* cube = new Cube();
    cube->translate(glm::vec3(0.0f, 0.0f, -5.0f));

    shader = new Shader("./resources/vert.glsl", "./resources/frag.glsl");
    camera = new Camera(1366, 768);

    camera->set_position(glm::vec3(-10.0f, 2.0f, 3.0f));

    while(running) {
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    // TODO: move to camera class, add speed, down up detection; OR: scripting?
                    //Movement
                    if(event.key.keysym.sym == SDLK_w) { //forward
                        camera->set_position(camera->position + camera->direction);
                    }
                    if(event.key.keysym.sym == SDLK_s) { //back
                        camera->set_position(camera->position - camera->direction);
                    }
                    if(event.key.keysym.sym == SDLK_a) { //right
                        camera->set_position(camera->position + camera->right);
                    }
                    if(event.key.keysym.sym == SDLK_d) { //left
                        camera->set_position(camera->position - camera->right);
                    }
                    if(event.key.keysym.sym == SDLK_q) { //down
                        camera->set_position(camera->position - glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                    if(event.key.keysym.sym == SDLK_e) { //up
                        camera->set_position(camera->position + glm::vec3(0.0f, 1.0f, 0.0f));
                    }

                    if(event.key.keysym.sym == SDLK_c) { //capture toggle
                        mouse_captured = !mouse_captured;
                        SDL_SetRelativeMouseMode(mouse_captured ? SDL_TRUE : SDL_FALSE);
                        spdlog::debug("Mouse capture: {}", mouse_captured);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        spdlog::debug("Mouse button down at ({}, {})", event.button.x, event.button.y);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        spdlog::debug("Mouse button up at ({}, {})", event.button.x, event.button.y);
                    }
                    break;

                //Mouse motion
                case SDL_MOUSEMOTION:
                    if(!mouse_captured) { //Ignore mouse motion if not captured
                        break;
                    }
                    //Camera rotation
                    camera->angle_x -= event.motion.xrel * 0.01f;
                    camera->angle_y += event.motion.yrel * 0.01f;

                    //Clamp y angle
                    if(camera->angle_y > 1.57f) {
                        camera->angle_y = 1.57f;
                    }
                    break;

                default:
                    break;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        ImGui::Begin("Debug");
        ImGui::Text("Camera position: (%f, %f, %f)", camera->position.x, camera->position.y, camera->position.z);
        ImGui::Text("Camera direction: (%f, %f, %f)", camera->direction.x, camera->direction.y, camera->direction.z);
        ImGui::Text("Camera right: (%f, %f, %f)", camera->right.x, camera->right.y, camera->right.z);
        ImGui::Text("Camera angle_x: %f", camera->angle_x);
        ImGui::Text("Camera angle_y: %f", camera->angle_y);
        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, 1366, 768);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        axis_widget->render();

        shader->use();
        cube->render();
        shader->drop();

        //Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(this->window);

        camera->update();
    }
} // OpenGLRenderer::run
