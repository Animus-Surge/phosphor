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

#include "phosphor/event.h"
#include "phosphor/mesh/axis_widget.h"
#include "phosphor/mesh/cube.h"
#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"
#include "phosphor/shader.h"
#include "phosphor/camera.h"

bool running = true;
Shader* shader;
Camera* camera;

//Camera variables
unsigned int camera_ubo;

glm::mat4 camera_view_matrix;
glm::mat4 camera_projection_matrix;
glm::mat4 camera_pv_matrix;

float camera_fov = 45.0f;
float camera_aspect_ratio = 1366.0f/768.0f;
float camera_near_clip = 0.1f;
float camera_far_clip = 100.0f;
float camera_angle_x = M_PI;
float camera_angle_y = 0.0f;

glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 camera_direction;
glm::vec3 camera_right;
glm::vec3 camera_up;

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

void regen_camera() {
    //Create UBO
    glGenBuffers(1, &camera_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, nullptr, GL_STATIC_DRAW);

    //Bind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, camera_ubo);

    //Create camera matrices
    camera_direction = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_right = glm::normalize(glm::cross(camera_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

    camera_view_matrix = glm::lookAt(
            camera_position, 
            camera_position + camera_direction, 
            glm::vec3(0.0f, 1.0f, 0.0f));
    camera_projection_matrix = glm::perspective(
            camera_fov, 
            camera_aspect_ratio, 
            camera_near_clip, 
            camera_far_clip);
    camera_pv_matrix = camera_projection_matrix * camera_view_matrix;

    //Rebind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, &camera_pv_matrix, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, camera_ubo);
}

void OpenGLRenderer::sigterm() {
    running = false;
    shutdown();
}

void OpenGLRenderer::run() {
    //TODO: add a debug reset keybind
    //TODO: event controller
    SDL_Event event;

    bool mouse_captured = false;

    AxisWidget* axis_widget = new AxisWidget();
    Cube* cube = new Cube();
    cube->translate(glm::vec3(0.0f, 0.0f, 4.0f));

    shader = new Shader("./resources/vert.glsl", "./resources/frag.glsl");
    camera = new Camera(1366, 768);

    float delta_time = 0.0f;

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
            camera_velocity += camera->direction;
        } 
        if(is_keydown(SDL_SCANCODE_S)) {
            camera_velocity -= camera->direction;
        } 
        if(is_keydown(SDL_SCANCODE_A)) {
            camera_velocity -= camera->right;
        } 
        if(is_keydown(SDL_SCANCODE_D)) {
            camera_velocity += camera->right;
        }
        if(is_keydown(SDL_SCANCODE_SPACE)) {
            camera_velocity += glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if(is_keydown(SDL_SCANCODE_LCTRL)) {
            camera_velocity -= glm::vec3(0.0f, 1.0f, 0.0f);
        }
        
        if(glm::length(camera_velocity) > 0.0f) {
            camera_velocity = glm::normalize(camera_velocity)  * delta_time;
            camera->translate(camera_velocity);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        SDL_version version;
        SDL_GetVersion(&version);

        ImGui::NewFrame();

        ImGui::Begin("Debug");

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Delta: %f", delta_time);
        ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
        ImGui::Text("OpenGL Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("SDL Version: %d.%d.%d", version.major, version.minor, version.patch);

        ImGui::Text("X-axis: %f", get_axis(SDL_SCANCODE_A, SDL_SCANCODE_D));
        ImGui::Text("Z-axis: %f", get_axis(SDL_SCANCODE_W, SDL_SCANCODE_S));
 
        if(ImGui::CollapsingHeader("Camera Info")) {
            ImGui::BeginChild("camera scroll");

            ImGui::Text("Camera Velocity: (%f, %f, %f)", camera_velocity.x, camera_velocity.y, camera_velocity.z);

            glm::vec3 position = camera->position;
            ImGui::Text("Camera Position: (%f, %f, %f)", position.x, position.y, position.z);

            glm::vec3 direction = camera->direction;
            ImGui::Text("Camera Direction: (%f, %f, %f)", direction.x, direction.y, direction.z);

            glm::vec3 right = camera->right;
            ImGui::Text("Camera Right: (%f, %f, %f)", right.x, right.y, right.z);

            glm::vec3 up = camera->up;
            ImGui::Text("Camera Up: (%f, %f, %f)", up.x, up.y, up.z);

            float angle_x = camera->angle_x;
            ImGui::Text("Camera Angle X: %f", angle_x);

            float angle_y = camera->angle_y;
            ImGui::Text("Camera Angle Y: %f", angle_y);

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

        //Timing
        float frame_end = SDL_GetTicks();
        delta_time = (frame_end - frame_start) / 1000.0f;
        
    } //while(running) -- Main loop

} // OpenGLRenderer::run
