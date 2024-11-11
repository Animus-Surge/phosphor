/*
 * Phosphor Test - main.cpp
 * Main entry point for the Phosphor Test application.
 */

#include "phosphor/core.h"

int main() {
    Phosphor* engine = new Phosphor(800, 600, "Phosphor Test");

    engine->init();
    engine->run();
    engine->shutdown();

    delete engine;

    return 0;
}
