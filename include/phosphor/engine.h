#pragma once

/**
 * Phosphor phosphor/engine.h
 * Engine systems and declarations
 */

class Engine {
public:
    Engine();
    ~Engine();

    void init();
    void shutdown();
    void run();
};
