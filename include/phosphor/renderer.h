#pragma once

/**
 * Phosphor phosphor/renderer.h
 * Renderer system and base class declaration
 */

#include <memory>

class Renderer {
public:
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void render() = 0;
}; // class Renderer

std::unique_ptr<Renderer> create_renderer(int type);
