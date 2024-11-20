#pragma once

/*
 * Phosphor phosphor/sdl/rendersys.h
 * Primitives rendering system header file for SDL
 */

#ifdef PHOSPHOR_SDL

#include <SDL2/SDL.h>

namespace phosphor::sdl {

void draw_point(SDL_Renderer* renderer, int x, int y);
void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h);
void draw_polyline(SDL_Renderer* renderer, SDL_Point* points, int num_points, bool closed = false);
void draw_circle(SDL_Renderer* renderer, int x, int y, int r);
void draw_ellipse(SDL_Renderer* renderer, int x, int y, int rx, int ry);
void draw_curve(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3);

void draw_fill_rect(SDL_Renderer* renderer, int x, int y, int w, int h);
void draw_fill_polyline(SDL_Renderer* renderer, SDL_Point* points, int num_points);
void draw_fill_circle(SDL_Renderer* renderer, int x, int y, int r);
void draw_fill_ellipse(SDL_Renderer* renderer, int x, int y, int rx, int ry);

#endif // PHOSPHOR_SDL
    
} // namespace phosphor::sdl
