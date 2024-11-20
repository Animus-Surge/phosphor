#pragma once

/*
 * Phosphor phosphor/object.h
 * Object classes
 */

#include <map>
#include <string>
#include <vector>

#include "phosphor/core.h"

class Object {
private:
    std::string name;
    std::map<std::string, Object*> children;
    std::map<std::string, Component*> components;
    Object* parent;
public:
    Object() {
        this->parent = nullptr;
    }
    ~Object() {
        for(auto& child : this->children) {
            delete child.second;
        }
    }

    //Child management
    void add_child(std::string name, Object* child) {
        this->children[name] = child;
        child->parent = this;
    }

    Object* get_child(std::string name) {
        return this->children[name];
    }

    void remove_child(std::string name) {
        this->children.erase(name);
    }

    //Component management
    void add_component(std::string name, Component* component) {
        //TODO: check if component with name already exists
        this->components[name] = component;
    }

    Component* get_component_by_name(std::string name) {
        return this->components[name];
    }

    void remove_component(std::string name) {
        this->components.erase(name);
    }

    //Update functions
    void update() {
        for(auto& component : this->components) {
            component.second->update();
        }
        for(auto& child : this->children) {
            child.second->update();
        }
    }

    void fixed_update(float delta) {
        for(auto& component : this->components) {
            component.second->fixed_update(delta);
        }
        for(auto& child : this->children) {
            child.second->fixed_update(delta);
        }
    }

    void render(SDL_Renderer* renderer) {
        for(auto& component : this->components) {
            component.second->render(renderer);
        }
        for(auto& child : this->children) {
            child.second->render(renderer);
        }
    }
}; // class Object
