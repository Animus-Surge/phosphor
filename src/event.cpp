/**
 * Phosphor event.cpp
 * Event manager implementation
 */

#include <SDL2/SDL.h>

#include "phosphor/event.h"

bool keystates[SDL_NUM_SCANCODES];
bool mousestates[8];

SDL_Point mouse_position = {0, 0}; //Set a default value to avoid null pointer exceptions

void handle_events(SDL_Event event) {
    switch(event.type) {
        case SDL_KEYDOWN:
            keystates[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            keystates[event.key.keysym.scancode] = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mousestates[event.button.button] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mousestates[event.button.button] = false;
            break;

        case SDL_MOUSEMOTION: {
            mouse_position.x = event.motion.x;
            mouse_position.y = event.motion.y;
            break; }
    }
}

float get_axis(SDL_Scancode positive, SDL_Scancode negative) { //TODO: action strength
    if(keystates[positive] && !keystates[negative]) {
        return 1.0f;
    } else if(keystates[negative] && !keystates[positive]) {
        return -1.0f;
    } else {
        return 0.0f;
    }
}

bool is_keydown(SDL_Scancode key) {
    return keystates[key];
}

bool is_mousedown(int button) {
    return mousestates[button];
}


