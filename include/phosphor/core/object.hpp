#pragma once

/**
 * Phosphor phosphor/core/object.hpp
 * Object class
 */

#include <string>
#include <vector>

#include "phosphor/core/component.hpp"

class Object {
protected:
    std::string name = "Object";
    std::vector<Component*> components;

public:
    Object();
    Object(std::string name);
    ~Object();

    void add_component(Component* component);
    void remove_component(Component* component);
    
    std::string get_name();
    void set_name(std::string name);

    void init() {
        for(auto component : components) {
            component->init();
        }
    }
    void update() {
        for(auto component : components) {
            component->update();
        }
    }
    void fixed_update(float delta) {
        for(auto component : components) {
            component->fixedUpdate(delta);
        }
    }
    void render() {
        for(auto component : components) {
            component->render();
        }
    }
    void shutdown() {
        for(auto component : components) {
            component->shutdown();
        }
    }

}; // class Object
