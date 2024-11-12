#pragma once

/*
 * Phosphor phosphor/core.h
 * Core header file
 *
 * Contains declarations for all core engine systems
 */

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <vector>
#include <map>

#define PHOSPHOR_ENGINE_DEFAULT_TITLE "Phosphor"

// Core functions
SDL_Point linear_interpolate(SDL_Point p1, SDL_Point p2, float t);

template <typename T>
T linear_interpolate(T p1, T p2, float t) {
    return p1 + (p2 - p1) * t;
}

// Phosphor engine class
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
