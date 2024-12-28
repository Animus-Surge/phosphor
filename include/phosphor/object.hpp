#pragma once

/**
* Phosphor include/phosphor/object.hpp
* Object base class
*/

#include <string>
#include <vector>

class Object;

class Component {
protected:
    Object* parent;

public:
    Component() {}
    virtual ~Component() {}

    virtual void init() {}
    virtual void update() {}
    virtual void fixedUpdate(float dt) {}
    virtual void render() {}
};

class Object {
protected:
    std::string name;
    std::vector<Object*> children;
    std::vector<Component*> components;
public:
    Object() = default;
    ~Object() {
        for (auto child : children) {
            delete child;
        }
    }

    void addChild(Object* child) {
        children.push_back(child);
    }
    void addComponent(Component* component) {
        components.push_back(component);
    }

    void update() {
        for (auto component : components) {
            component->update();
        }
        for (auto child : children) {
            child->update();
        }
    }

    void fixedUpdate(float dt) {
        for (auto component : components) {
            component->fixedUpdate(dt);
        }
        for (auto child : children) {
            child->fixedUpdate(dt);
        }
    }

    void render() {
        for (auto component : components) {
            component->render();
        }
        for (auto child : children) {
            child->render();
        }
    }
};