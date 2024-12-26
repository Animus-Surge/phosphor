#pragma once

/**
 * Phosphor include/phosphor/backends/vulkan_renderer.hpp
 * Vulkan renderer backend for Phosphor.
 */

#include <vector>

#include <vulkan/vulkan.hpp>

#include "phosphor/backends/renderer.hpp"

class VulkanRenderer : public Renderer {
    protected:
        vk::Instance instance;
    public:
        ~VulkanRenderer();

        int init() override;
        void render() override;

        bool checkSupport();
        bool attemptCreateDevice();
};

