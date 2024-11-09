#pragma once

/*
 * Phosphor phosphor/core.h
 * Core header file
 *
 * Contains declarations for all core engine systems
 */

#include <SDL2/SDL.h>
#include <GL/gl.h>

#define PHOSPHOR_ENGINE_DEFAULT_TITLE "Phosphor"

class Phosphor {

public:
    Phosphor(int width, int height, const char* title = PHOSPHOR_ENGINE_DEFAULT_TITLE) {
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
