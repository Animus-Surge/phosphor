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

#include <SDL2/SDL.h>

namespace phosphor {

    // Scene class
    class Scene {
        private:
            std::string name;
            std::map<std::string, Object*> objects;

        public:
            Scene() {};
            ~Scene() {};

            void set_name(std::string name) {
                this->name = name;
            }
            std::string get_name() {
                return this->name;
            }

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

            //Update all objects in the scene
            void update() {
                for (auto& object : objects) {
                    object.second->update();
                }
            }

            //Fixed update all objects in the scene
            void fixed_update(float delta) {
                for (auto& object : objects) {
                    object.second->fixed_update(delta);
                }
            }

            //Render all objects in the scene
            void render(SDL_Renderer* renderer) {
                for (auto& object : objects) {
                    object.second->render(renderer);
                }
            }

    }; // class Scene

} // namespace phosphor
