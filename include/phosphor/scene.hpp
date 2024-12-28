#pragma once

/**
* Phosphor include/phosphor/scene.hpp
* Scene class
 */

#include <string>
#include <vector>

#include "phosphor/object.hpp"

class Scene {
protected:
    std::string name;
    std::vector<Object*> objects;
public:
    Scene() = default;
    ~Scene() {
        for (auto object : objects) {
            delete object;
        }
    }

    void addObject(Object* object) {
        objects.push_back(object);
    }

    void update() {
        for (auto object : objects) {
            object->update();
        }
    }

    void fixedUpdate(float dt) {
        for (auto object : objects) {
            object->fixedUpdate(dt);
        }
    }

    void render() {
        for (auto object : objects) {
            object->render();
        }
    }
};