/**
 * Phosphor renderer/opengl_renderer.cpp
 * OpenGL renderer implementation
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "phosphor/renderer.h"
#include "phosphor/backends/opengl_renderer.h"

void OpenGLRenderer::init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(SDL_GL_GetCurrentWindow());
}

void OpenGLRenderer::shutdown() {
    SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
    SDL_DestroyWindow(SDL_GL_GetCurrentWindow());
    SDL_Quit();
}

void OpenGLRenderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //TODO: scene rendering

    SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}


