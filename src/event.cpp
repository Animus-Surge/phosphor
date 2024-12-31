/**
* Phosphor src/event.cpp
* Event function implementations
*/

#include <map>
#include <utility>

#include "phosphor/event.hpp"

bool keystate[SDL_NUM_SCANCODES] = {false};
bool mousestate[8] = {false};

std::map<SDL_Scancode, std::function<void()>> key_callbacks;
std::map<unsigned long, std::function<void()>> system_callbacks;

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
    unsigned long type = event->type;
    //Check against system callback map
    if (system_callbacks.find(event->type) != system_callbacks.end()) {
        system_callbacks[event->type]();
    }

    //Keyboard events
    if (event->type == SDL_KEYDOWN) {
        const SDL_Scancode sc = event->key.keysym.scancode;
        set_keystate(sc, true);

        //Callback
        if (key_callbacks.find(sc) != key_callbacks.end()) {
            key_callbacks[sc]();
        }
    } else if (event->type == SDL_KEYUP) {
        const SDL_Scancode sc = event->key.keysym.scancode;
        set_keystate(sc, false);
    }
}

void add_key_callback(const SDL_Scancode key, unsigned char state, std::function<void()> callback) {
    //TODO: state checking
    key_callbacks[key] = std::move(callback);
}

void add_system_callback(const unsigned long type, std::function<void()> callback) {
    system_callbacks[type] = std::move(callback);
}