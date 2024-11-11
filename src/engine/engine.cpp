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

void _sdl_run(SDL_Renderer* renderer) {

    SDL_Event event;

    while(true) {

        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        draw_point(renderer, 10, 10);
        draw_line(renderer, 20, 10, 110, 100);
        draw_rect(renderer, 120, 10, 100, 100);

        SDL_Point points[] = {{230, 10}, {330, 10}, {330, 110}, {230, 110}};
        draw_polyline(renderer, points, 4, true);
        draw_circle(renderer, 400, 60, 50);
        draw_ellipse(renderer, 510, 60, 50, 30);
        draw_curve(renderer, 600, 10, 700, 110, 800, 10);

        draw_fill_rect(renderer, 10, 120, 100, 100);
        draw_fill_polyline(renderer, points, 4);

        SDL_RenderPresent(renderer);

    }

}



void _gl_run() {}

void Phosphor::run() {
    _sdl_run(this->renderer);
}

void Phosphor::shutdown() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
