#pragma once

/*
 * Phosphor phosphor/engine.h
 * Engine declarations
 */

#include <SDL2/SDL.h>

#include "phosphor/scene.h"

#include <map>
#include <vector>

class Phosphor {

public:
    Scene* current_scene;

    void set_scene(Scene* scene) {
        this->current_scene = scene;
    }
    void load_scene(std::string path);

    Phosphor(int width, int height, const char* title = "Phosphor") {
        this->width = width;
        this->height = height;
        this->title = title;
    };
    ~Phosphor();

    void init();
    void run();
    void shutdown();

private:
    SDL_Window* window;
    const char* title;
    int width, height;
    SDL_Renderer* renderer;

}; // class Phosphor
