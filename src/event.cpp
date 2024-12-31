/**
* Phosphor src/event.cpp
* Event function implementations
*/

#include "phosphor/event.hpp"

bool keystate[SDL_NUM_SCANCODES] = {false};
bool mousestate[8] = {false};

void set_keystate(SDL_Scancode key, bool state) {
    keystate[key] = state;
}

void set_mousestate(int button_idx, bool state) {
    mousestate[button_idx] = state;
}

bool get_keystate(SDL_Scancode key) {
    return keystate[key];
}

bool get_mousestate(int button_idx) {
    return mousestate[button_idx];
}

void handle_event(const SDL_Event* event) {

}

void add_key_callback(SDL_Scancode key, std::function<void()> callback) {}