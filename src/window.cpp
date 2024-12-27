/**
 * Phosphor src/window.cpp
 * Window class implementation.
 */

#include "phosphor/window.hpp"

#include <spdlog/spdlog.h>

void Window::init(unsigned int flags) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to initialize SDL: {}", error);
        return;
    }

    this->window = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_INITIAL_WIDTH, WINDOW_INITIAL_HEIGHT, flags);
    if (this->window == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to create window: {}", error);
        this->window = nullptr;
        return;
    }

    spdlog::info("Window created successfully.");
    //Context creation handled by renderer subclasses
}

Window::~Window() {
    if (this->window != nullptr) {
        SDL_DestroyWindow(this->window);
    }
}

//Accessors

const char* Window::getTitle() {
    return SDL_GetWindowTitle(this->window);
}

int Window::getX() {
    int x;
    SDL_GetWindowPosition(this->window, &x, nullptr);
    return x;
}

int Window::getY() {
    int y;
    SDL_GetWindowPosition(this->window, nullptr, &y);
    return y;
}

int Window::getWidth() {
    int w;
    SDL_GetWindowSize(this->window, &w, nullptr);
    return w;
}

int Window::getHeight() {
    int h;
    SDL_GetWindowSize(this->window, nullptr, &h);
    return h;
}

bool Window::isFullscreen() {
    return SDL_GetWindowFlags(this->window) & SDL_WINDOW_FULLSCREEN;
}

//Modifiers

void Window::setTitle(const char* title) {
    SDL_SetWindowTitle(this->window, title);
}

void Window::setPosition(int x, int y) {
    SDL_SetWindowPosition(this->window, x, y);
}

void Window::setSize(int w, int h) {
    SDL_SetWindowSize(this->window, w, h);
}

void Window::setFullscreen(bool fullscreen) {
    if (fullscreen) {
        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
    } else {
        SDL_SetWindowFullscreen(this->window, 0);
    }
}

