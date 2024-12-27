#pragma once

/**
 * Phosphor include/phosphor/backends/vulkan_renderer.hpp
 * Vulkan renderer backend for Phosphor.
 */

#include <vector>

#include <vulkan/vulkan.hpp>

#include "phosphor/backends/renderer.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

class VulkanRenderer : public Renderer {
    private:
        void getDevice();
        void createPipeline();
        void createSwapchain();
        void createCommandBuffers();

    protected:
        //Instance
        vk::Instance instance;

        //Device
        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        //Rendering resources
        vk::Queue graphicsQueue;
        vk::SurfaceKHR surface;
        vk::RenderPass renderPass;
        vk::PipelineLayout pipelineLayout;
        vk::Pipeline graphicsPipeline;

        //Framebuffer
        std::vector<vk::Framebuffer> framebuffers;

        //Swapchain
        vk::SwapchainKHR swapchain;
        std::vector<vk::Image> swapchainImages;
        std::vector<vk::ImageView> swapchainImageViews;
        vk::Format swapchainImageFormat;
        vk::Extent2D swapchainExtent;

        //Command buffers
        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;

        //Semaphores and Fences
        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;

    public:
        ~VulkanRenderer();

        int init() override;
        void render() override;

        bool checkSupport();
        bool attemptCreateDevice();
};

