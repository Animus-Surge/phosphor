/**
 * Phosphor renderer/vulkan_renderer.cpp
 * Vulkan renderer implementation.
 */

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

#include "phosphor/backends/vulkan_renderer.hpp"

void VulkanRenderer::init() {
    //SDL initialization
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to initialize SDL: {}", SDL_GetError());
        return;
    }

    this->window = SDL_CreateWindow("Phosphor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    if(this->window == nullptr) {
        std::string error = SDL_GetError();
        spdlog::critical("Failed to create window: {}", error);
        SDL_Quit();
        return;
    }

    //Get Vulkan extensions
    unsigned int ext_ct = 0;
    SDL_Vulkan_GetInstanceExtensions(this->window, &ext_ct, nullptr);
    std::vector<const char*> extensions(ext_ct);
    SDL_Vulkan_GetInstanceExtensions(this->window, &ext_ct, extensions.data());

    vk::ApplicationInfo appInfo{};

    vk::InstanceCreateInfo createInfo{};

}
