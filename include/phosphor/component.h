#pragma once

/*
 * Phosphor phosphor/component.h
 * Component base class
 */

#include <vector>
#include <string>

#include "phosphor/core.h"

#include <SDL2/SDL.h>

class Object;

class Component {
private:
    std::string name;
    Object* parent;

public:
    Component() {
    }
    ~Component() {
    }

    void set_parent(Object* object) {
        this->parent = object;
    }
    Object* get_parent() {
        return this->parent;
    }

    void set_name(std::string newname) {
        this->name = newname;
    }
    std::string get_name() {
        return this->name;
    }

    virtual void input() = 0;
    virtual void update() = 0;
    virtual void fixed_update(float delta) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

}; // class Component

class Transform : public Component {
public:
    SDL_Point position = {0, 0};
    double rotation = 0.0;
    SDL_FPoint scale = {0.0f, 0.0f};

    Transform() {}
    ~Transform() {}

    void input() override {}
    void update() override {}
    void fixed_update(float delta) override {}
    void render(SDL_Renderer* renderer) override {}

};

class PrimitiveShape : public Component {
private:
    SDL_Color fill_color = {255, 255, 255, 255}; //TODO: replace with a ColorValue object which can handle gradients
    SDL_Color border_color = {255, 255, 255, 255};
    int border_width = 1;

    bool fill = false;

    /*
     * 0: polygon
     * 1: curve
     * 2: line
     * 3: ellipse/circle
     * 4: rect
     */
    int type = 0;

    //Fields specific to Polygons/Polylines
    std::vector<SDL_Point> points;
    bool close = true;

    //Fields specific to Rectangles
    int width, height;
    SDL_Point position;

    //Overrideables
    void input() override;
    void update() override;
    void fixed_update(float delta) override;
    void render(SDL_Renderer* renderer) override;

};
