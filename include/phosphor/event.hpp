#pragma once

/**
* Phosphor include/phosphor/event.hpp
* Event handling for Phosphor
*/

#include <functional>

#include <SDL2/SDL.h>

#include "phosphor/phosphor.hpp"

extern bool keystate[SDL_NUM_SCANCODES];
extern bool mousestate[8];

void set_keystate(SDL_Scancode key, bool state);
void set_mousestate(int button_idx, bool state);

bool get_keystate(SDL_Scancode key);
bool get_mousestate(int button_idx);

/**
* Handle a system event (i.e. window state events, sdl events, etc.)
* @param event The event to handle
*/
void handle_event(const SDL_Event* event);

/**
* Add a key callback when the specified key enters the specified state
* @param key The key scancode
* @param state The state to listen for
* @param callback The callback to run when the key is pressed
*/
void PHOSPHOR_EXPORT add_key_callback(SDL_Scancode key, unsigned char state, std::function<void()> callback);

/**
 * Add a callback for a system event
 * @param type The type of event to listen for
 * @param callback The callback to run
 */
void PHOSPHOR_EXPORT add_system_callback(unsigned long type, std::function<void()> callback);