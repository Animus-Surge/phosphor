#pragma once

/*
 * Phosphor phosphor/object.h
 * Object classes
 */

#include <map>
#include <string>
#include <vector>

#include "phosphor/component.h"

class Object {
private:
    std::map<std::string, Object*> children;
    //std::vector<Component*> components;
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
    void addChild(Object* child) {
        this->children[child->getName()] = child;
        child->setParent(this);
    }
    void removeChild(Object* child) {
        this->children.erase(child->getName());
        child->setParent(nullptr);
    }
    void setParent(Object* parent) {
        this->parent = parent;
    }
    Object* getParent() {
        return this->parent;
    }
    std::string getName() {
        return "Object";
    }
};
