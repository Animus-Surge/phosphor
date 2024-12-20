#pragma once

/**
 * Phosphor phosphor/core/component.hpp
 * Component class
 */

//#include "phosphor/core/object.hpp"

//class Object;

class Component {
protected:
//    Object* parent;

public:
    Component() {}
    ~Component() {}

//    void set_parent(Object* parent);
//    Object* get_parent();

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void fixedUpdate(float delta) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    //TODO: callback classes, event handling
}; // class Component
