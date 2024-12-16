/**
 * Phosphor phosphor.cpp
 *
 * Entry point and initialization for the Phosphor engine
 */

#include <signal.h>
#include <unistd.h>

#include "spdlog/spdlog.h"

#include "phosphor/phosphor.hpp"

std::unique_ptr<Renderer> renderer;

void signal_callback(int signum) {
    spdlog::info("Received signal {}", signum);

    switch(signum) {
        case SIGINT:
            spdlog::info("Shutting down"); //TODO
            renderer->sigterm();
            renderer->shutdown();
            exit(0);
            
        default:
            break;
    }
}

void phosphor_init() {
    // Register signal handlers
    signal(SIGINT, signal_callback);

    if(PHOSPHOR_DEBUG) {
        spdlog::set_level(spdlog::level::debug);
    } else {
        spdlog::set_level(spdlog::level::info);
    }

    spdlog::info("Phosphor version: {}", phosphor_version());

    // Initialize the renderer
    renderer = create_renderer(0);
    renderer->init();
    renderer->run();
    renderer->shutdown();

    spdlog::info("Phosphor shutting down");

}
