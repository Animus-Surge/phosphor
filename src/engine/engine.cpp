/*
 * Phosphor engine/engine.cpp
 * Engine definitions and implementation
 */

#include "phosphor/core.h"
#include "phosphor/sdl/rendersys.h"

#include <SDL2/SDL.h>

Phosphor::~Phosphor() {
}

void Phosphor::init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        exit(1);
    }

    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 0); //FIXME
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
}

void _sdl_run(Phosphor* engine, SDL_Renderer* renderer) {

    SDL_Event event;

    engine->current_scene = new Scene();

    float delta = 0.0f;

    while(true) {

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                break;
            }
        }

        //Updates
        engine->current_scene->update();
        engine->current_scene->fixed_update(delta);

        //Clear
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        engine->current_scene->render(renderer);
        
        SDL_RenderPresent(renderer);

        //Compute delta time
        delta = SDL_GetTicks();

    }

}



void _gl_run() {}

void Phosphor::run() {
    _sdl_run(this, this->renderer);
}

void Phosphor::shutdown() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
