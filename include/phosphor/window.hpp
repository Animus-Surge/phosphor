#pragma once

/**
 * Phosphor include/phosphor/window.hpp
 * Window class for Phosphor.
 */

#include <SDL2/SDL.h>

#define WINDOW_INITIAL_WIDTH 800
#define WINDOW_INITIAL_HEIGHT 600



class Window {
    protected: 
        SDL_Window* window;
    public:
        Window() {
            window = nullptr;
        }
        ~Window();

        void setTitle(const char* title);
        void setPosition(int x, int y);
        void setSize(int w, int h);
        void setFullscreen(bool fullscreen);

        void init(unsigned int flags);

        const char* getTitle();
        int getX();
        int getY();
        int getWidth();
        int getHeight();
        bool isFullscreen();

        SDL_Window* getSDLWindow() {
            return this->window;
        }
};

