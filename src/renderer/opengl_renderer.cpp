/**
 * Phosphor renderer/opengl_renderer.cpp
 * OpenGL renderer implementation
 */

#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include "phosphor/event.hpp"
#include "phosphor/mesh/axis_widget.hpp"
#include "phosphor/mesh/cube.hpp"
#include "phosphor/light/point_light.hpp"
#include "phosphor/renderer.hpp"
#include "phosphor/backends/opengl_renderer.hpp"
#include "phosphor/shader.hpp"
#include "phosphor/camera.hpp"

bool running = true;

Camera* camera;
Shader* shader;

AxisWidget* axis;

Mesh* test_mesh;

//TODO: OpenGL debug callback

void OpenGLRenderer::init() {
    spdlog::info("Creating OpenGL Renderer...");
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::string error = SDL_GetError();
        spdlog::critical("SDL_Init: {}", error);
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    this->window = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_OPENGL);
    if(this->window == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("SDL_CreateWindow: {}", error);
        return;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if(this->context == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("SDL_GL_CreateContext: {}", error);
        return;
    }
    SDL_GL_MakeCurrent(this->window, this->context);


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
    shutdown();
}

void render_debug_modal(float delta_time, glm::vec3 camera_velocity, bool mouse_captured) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Tools");

    //Get version
    SDL_version version;
    SDL_GetVersion(&version);

    if(ImGui::CollapsingHeader("Debug Info")) {
        ImGui::Text("Phosphor Engine");
        ImGui::Text("Version: 0.0.1");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Delta: %f", delta_time);
        ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
        ImGui::Text("OpenGL Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("SDL Version: %d.%d.%d", version.major, version.minor, version.patch);
    }

    if(ImGui::CollapsingHeader("Camera Info")) {
        ImGui::BeginChild("camera scroll");

        ImGui::Text("Camera Velocity: (%f, %f, %f)", camera_velocity.x, camera_velocity.y, camera_velocity.z);

        glm::vec3 position = camera->get_position();
        ImGui::Text("Camera Position: (%f, %f, %f)", position.x, position.y, position.z);

        glm::vec3 direction = camera->get_direction();
        ImGui::Text("Camera Direction: (%f, %f, %f)", direction.x, direction.y, direction.z);

        glm::vec3 right = camera->get_right();
        ImGui::Text("Camera Right: (%f, %f, %f)", right.x, right.y, right.z);

        glm::vec3 up = camera->up;
        ImGui::Text("Camera Up: (%f, %f, %f)", up.x, up.y, up.z);

        float angle_x = camera->angle_x;
        ImGui::Text("Camera Angle X: %f", angle_x);

        float angle_y = camera->angle_y;
        ImGui::Text("Camera Angle Y: %f", angle_y);

        float fov = camera->cameraData.cam_fov;
        ImGui::Text("Camera FOV: %f", fov);

        float aspect_ratio = camera->cameraData.cam_aspect_ratio;
        ImGui::Text("Camera Aspect Ratio: %f", aspect_ratio);

        float near_clip = camera->cameraData.cam_near_clip;
        ImGui::Text("Camera Near Clip: %f", near_clip);

        float far_clip = camera->cameraData.cam_far_clip;
        ImGui::Text("Camera Far Clip: %f", far_clip);

        if(ImGui::CollapsingHeader("Camera Matrices")) {
            ImGui::Text("View Matrix");
            glm::mat4 view_matrix = camera->get_view_matrix();
            for(int i = 0; i < 4; i++) {
                ImGui::Text("%f %f %f %f", view_matrix[i][0], view_matrix[i][1], view_matrix[i][2], view_matrix[i][3]);
            }

            ImGui::Text("Projection Matrix");
            glm::mat4 projection_matrix = camera->get_projection_matrix();
            for(int i = 0; i < 4; i++) {
                ImGui::Text("%f %f %f %f", projection_matrix[i][0], projection_matrix[i][1], projection_matrix[i][2], projection_matrix[i][3]);
            }

            ImGui::Text("PV Matrix");
            glm::mat4 pv_matrix = camera->get_pv_matrix();
            for(int i = 0; i < 4; i++) {
                ImGui::Text("%f %f %f %f", pv_matrix[i][0], pv_matrix[i][1], pv_matrix[i][2], pv_matrix[i][3]);
            }
        } //Camera Matrices

        ImGui::EndChild();
    } //Camera Info

    if(ImGui::CollapsingHeader("Input")) {
        ImGui::Text("Mouse Capture: %s", mouse_captured ? "true" : "false");
        ImGui::Text("Mouse Position: (%d, %d)", get_mouse_position().x, get_mouse_position().y);
    }

    ImGui::End();

    ImGui::Render();

}

void OpenGLRenderer::run() {
    SDL_Event event;

    bool mouse_captured = false;

    //Lights

    //Engine-specific widgets
    axis = new AxisWidget();

    PointLight* light = new PointLight();
    light->set_position(glm::vec3(1.0f, 1.0f, 1.0f));
    light->gen_ubo();

    test_mesh = new Cube();
    test_mesh->translate(glm::vec3(0.0f, 0.0f, 4.0f));

    glm::vec3 camera_initial_position = glm::vec3(0.0f, 0.0f, -4.0f);

    //Camera and mesh shader (TODO: move mesh shader to mesh class instead)
    shader = new Shader("./resources/vert.glsl", "./resources/frag.glsl");
    camera = new Camera(1366, 768);
    camera->set_position(camera_initial_position);

    float delta_time = 0.0f;

    //Set depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while(running) {
        float frame_start = SDL_GetTicks();

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

                    if(event.key.keysym.sym == SDLK_c) { //capture toggle
                        mouse_captured = !mouse_captured;
                        SDL_SetRelativeMouseMode(mouse_captured ? SDL_TRUE : SDL_FALSE);
                        //Lock cursor to center
                        if(mouse_captured) {
                            SDL_WarpMouseInWindow(this->window, 1366 / 2, 768 / 2);
                        }
                        spdlog::debug("Mouse capture: {}", mouse_captured);
                    }

                    if(event.key.keysym.sym == SDLK_r) { //reset
                                                         //Reset camera
                        mouse_captured = false;
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        camera = new Camera(1366, 768);
                        camera->set_position(camera_initial_position);
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
                    camera->rotate(event.motion.xrel, event.motion.yrel);
                    break;

                    //Window focus
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                        mouse_captured = false;
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
                    break;
                default:
                    break;
            }

            handle_events(event);
        } //if(SDL_PollEvent(&event)) -- Event loop

        //Lock cursor to center
        if(mouse_captured) {
            SDL_WarpMouseInWindow(this->window, 1366 / 2, 768 / 2);
        }

        //Updates
        glm::vec3 camera_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        if(is_keydown(SDL_SCANCODE_W)) {
            camera_velocity += camera->get_direction();
        }
        if(is_keydown(SDL_SCANCODE_S)) {
            camera_velocity -= camera->get_direction();
        }
        if(is_keydown(SDL_SCANCODE_A)) {
            camera_velocity -= camera->get_right();
        }
        if(is_keydown(SDL_SCANCODE_D)) {
            camera_velocity += camera->get_right();
        }
        if(is_keydown(SDL_SCANCODE_SPACE)) {
            camera_velocity += glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if(is_keydown(SDL_SCANCODE_LCTRL)) {
            camera_velocity -= glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if(glm::length(camera_velocity) > 0.0f) {
            camera_velocity = glm::normalize(camera_velocity)  * delta_time;
            if(is_keydown(SDL_SCANCODE_LSHIFT)) {
                camera_velocity *= 2.0f;
            }
            camera->translate(camera_velocity);
        }

        render_debug_modal(delta_time, camera_velocity, mouse_captured); //Render ImGui debug modal

        glViewport(0, 0, 1366, 768);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //TODO: gridlines

        light->render();

        axis->render();

        shader->use();
        test_mesh->render();
        shader->drop();

        //Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(this->window);

        //Timing
        float frame_end = SDL_GetTicks();
        delta_time = (frame_end - frame_start) / 1000.0f;

    } //while(running) -- Main loop

} // OpenGLRenderer::run
