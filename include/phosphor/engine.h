#pragma once

/**
 * Phosphor phosphor/engine.h
 * Engine systems and declarations
 */

#include <memory>

#include "phosphor/renderer.h"

class Engine {
    std::unique_ptr<Renderer> renderer;
public:
    Engine();
    ~Engine();

    void init();
    void shutdown();
    void run();
};
