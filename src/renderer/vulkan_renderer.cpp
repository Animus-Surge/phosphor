/**
 * Phosphor src/renderer/vulkan_renderer.cpp
 * Vulkan renderer backend implementation.
 */

#include "phosphor/backends/vulkan_renderer.hpp"

#include <spdlog/spdlog.h>

VulkanRenderer::~VulkanRenderer() {
    if (instance) {
        instance.destroy();
    }
}

void VulkanRenderer::init() {
    //TODO: Check for Vulkan support

    vk::ApplicationInfo appInfo(
            "Phosphor Engine",  //TODO: use game name instead, config
            VK_MAKE_VERSION(0, 1, 0),  //TODO: pull version from config
            "Phosphor", 
            VK_MAKE_VERSION(0, 1, 0), 
            VK_API_VERSION_1_0  );

    vk::InstanceCreateInfo createInfo {
        {}, //flags
            &appInfo
    };

    instance = vk::createInstance(createInfo);

    spdlog::info("Vulkan instance created.");
}

void VulkanRenderer::render() {
    //TODO
}

