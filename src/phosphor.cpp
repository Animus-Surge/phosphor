/**
 * Phosphor phosphor.cpp
 *
 * Entry point and initialization for the Phosphor engine
 */

#include <signal.h>
#include <unistd.h>

#include "spdlog/spdlog.h"

#include "phosphor/phosphor.h"

void signal_callback(int signum) {
    spdlog::info("Received signal {}", signum);

    switch(signum) {
        case SIGINT:
            spdlog::info("Shutting down"); //TODO
            break;
        default:
            break;
    }
}

void phosphor_init() {
    // Register signal handlers
    signal(SIGINT, signal_callback);


    spdlog::info("Phosphor version: {}", phosphor_version());

    // Initialize the renderer
    std::unique_ptr<Renderer> renderer = create_renderer(0);
    renderer->init();
    renderer->run();
    renderer->shutdown();

    spdlog::info("Phosphor shutting down");

}
