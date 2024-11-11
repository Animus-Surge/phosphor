#pragma once

/*
 * Phosphor phosphor/component.h
 * Component base class
 */

#include <string>

#include <SDL2/SDL.h>

class Component {
private:
    std::string name;
public:
    Component() {
    }
    ~Component() {
    }
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual std::string getName() = 0;

}; // class Component
