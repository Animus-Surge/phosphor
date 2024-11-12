#pragma once

/*
 * Phosphor phosphor/scene.h
 * Scene header file
 *
 * Contains declarations for all scene-related classes
 */

#include <map>
#include <string>

#include "phosphor/object.h"

// Scene class
class Scene {
private:
    //Scene objects, mapped by name
    std::map<std::string, Object*> objects;

public:
    Scene() {};
    ~Scene() {};

    //Add an object to the scene
    void add_object(std::string name, Object* object) {
        objects[name] = object;
    }

    //Get an object from the scene
    Object* get_object(std::string name) {
        return objects[name];
    }

    //Remove an object from the scene
    void remove_object(std::string name) {
        objects.erase(name);
    }

    //TODO: render, update, fixed_update

}; // class Scene
