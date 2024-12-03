#pragma once

/**
 * Phosphor phosphor/event.h
 * Event manager
 */

#include <SDL2/SDL.h>

//Event handling
void handle_events(SDL_Event event);

//States
bool is_keydown(SDL_Scancode key);
bool is_mousedown(int button);

//Axis input
float get_axis(SDL_Scancode positive, SDL_Scancode negative);

//Mouse position
int get_mouse_x();
int get_mouse_y();


