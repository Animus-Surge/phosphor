/**
 * Phosphor engine.cpp
 *
 * Engine systems and declarations
 */

#include <cstdio>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

#include "phosphor/engine.h"
#include "phosphor/renderer.h"

using json = nlohmann::json;

//TODO: integrate spdlog

Engine::Engine() {
    // Steps for initializing the engine:
    //TODO: load from main header; move file functions to separate header

    std::string config_file = "/home/surge/.config/phosphor/config.json";

    //1. Check for configuration file
    std::ifstream file(config_file);
    if (!file.is_open()) {
        //TODO: create default configuration file
    } else {
        //2. Load configuration file
        json config;
        file >> config;

        //3. Load configuration values
        //TODO: config schema
    }

    //4. Initialize renderer
    //TODO: renderer type
    this->renderer = create_renderer(0);
}

Engine::~Engine() {
}

void Engine::init() {
    this->renderer->init();
}

void Engine::shutdown() {
    this->renderer->shutdown();
}

void Engine::run() {
    this->init();

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        this->renderer->render();

    } 
    
    this->shutdown();
}
