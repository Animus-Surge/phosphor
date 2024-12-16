#pragma once

/**
 * Phosphor phosphor/renderer.h
 * Renderer system and base class declaration
 */

#include <any>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Renderer {
protected:
    SDL_Window* window;
public:
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void run() = 0;

    // Signal handler (TODO: replace with generic handler)
    virtual void sigterm() = 0;
}; // class Renderer

std::unique_ptr<Renderer> create_renderer(int type);
