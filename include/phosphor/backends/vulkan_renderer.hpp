#pragma once

/**
 * Phosphor phosphor/backends/vulkan_renderer.hpp
 * Vulkan renderer class
 */

#include <string>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "phosphor/renderer.hpp"

class VulkanRenderer : public Renderer {
protected:
    vk::Instance instance;

public:
    void init() override;
    void shutdown() override;
    void run() override;

    void sigterm() override;
}; // class VulkanRenderer


