/**
 * Phosphor src/renderer/vulkan_renderer.cpp
 * Vulkan renderer backend implementation.
 */

#ifdef PHOSPHOR_VULKAN

#include "phosphor/backends/vulkan_renderer.hpp"
#include "phosphor/filesystem.hpp"

#include <vector>
#include <functional>
#include <stdexcept>
#include <cstdlib>
#include <optional>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

// Utility functions

/**
 * Gets the Vulkan-compatible device to use.
 */
void VulkanRenderer::getDevice() {
    auto devices = this->instance.enumeratePhysicalDevices();

    for (const auto& device : devices) {
        QueueFamilyIndices indices = findQueueFamilies(device);
        if(indices.isComplete()) {
            physicalDevice = device;
            break;
        }
    }

    if(!physicalDevice) {
        spdlog::error("Failed to find a suitable GPU.");
        throw std::runtime_error("Failed to find a suitable GPU."); //TODO: Custom exception, error handling and switching to OpenGL instance instead
    }

    auto deviceIndices = findQueueFamilies(physicalDevice);
    float priority = 1.0f;

    vk::DeviceQueueCreateInfo queueCreateInfo {
        {}, //flags
            deviceIndices.graphicsFamily.value(),
            1,
            &priority
    };

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo {
        {}, //flags
            1,
            &queueCreateInfo,
            0,
            nullptr,
            0,
            nullptr,
            &deviceFeatures
    };

    device = physicalDevice.createDevice(createInfo);
    graphicsQueue = device.getQueue(deviceIndices.graphicsFamily.value(), 0);
}

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;

    auto queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for(const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        if(indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

/**
 * Creates the rendering pipeline
 */
void VulkanRenderer::createPipeline() {
    spdlog::info("VULKAN: Creating pipeline...");

    auto vertShaderCode = readBinFile("shaders/test.vert.spv");
    auto fragShaderCode = readBinFile("shaders/test.frag.spv");

    vk::ShaderModuleCreateInfo vertShaderCreateInfo {
        {}, //flags
            vertShaderCode.size(),
            reinterpret_cast<const uint32_t*>(vertShaderCode.data())
    };
    vk::ShaderModuleCreateInfo fragShaderCreateInfo {
        {}, //flags
            fragShaderCode.size(),
            reinterpret_cast<const uint32_t*>(fragShaderCode.data())
    };

    vk::ShaderModule vertShaderModule = device.createShaderModule(vertShaderCreateInfo);
    vk::ShaderModule fragShaderModule = device.createShaderModule(fragShaderCreateInfo);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo {
        {}, //flags
            vk::ShaderStageFlagBits::eVertex,
            vertShaderModule,
            "main"
    };
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo {
        {}, //flags
            vk::ShaderStageFlagBits::eFragment,
            fragShaderModule,
            "main"
    };

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    spdlog::debug("VULKAN: Shader modules created.");

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{{}, 0, nullptr, 0, nullptr};
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{{}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

    vk::Viewport viewport{0.0f, 0.0f, (float) swapchainExtent.width, (float) swapchainExtent.height, 0.0f, 1.0f};
    vk::Rect2D scissor{{0, 0}, swapchainExtent};

    vk::PipelineViewportStateCreateInfo viewportState{{}, 1, &viewport, 1, &scissor};

    spdlog::debug("VULKAN: Viewport and scissor created.");

    vk::PipelineRasterizationStateCreateInfo rasterizer{{}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f};

    vk::PipelineMultisampleStateCreateInfo multisampling{{}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE};

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{VK_FALSE, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::ColorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)};
    vk::PipelineColorBlendStateCreateInfo colorBlending{{}, VK_FALSE, vk::LogicOp::eCopy, 1, &colorBlendAttachment, {0.0f, 0.0f, 0.0f, 0.0f}};

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{{}, 0, nullptr, 0, nullptr};

    pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);

    spdlog::debug("VULKAN: Pipeline layout created.");

    vk::GraphicsPipelineCreateInfo pipelineInfo{{}, 2, shaderStages, &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, nullptr, &colorBlending, nullptr, pipelineLayout, renderPass, 0, VK_NULL_HANDLE, -1};

    graphicsPipeline = device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo).value;

    device.destroyShaderModule(vertShaderModule);
    device.destroyShaderModule(fragShaderModule);

    spdlog::info("VULKAN: Pipeline created.");

    //Framebuffers

    spdlog::info("VULKAN: Creating framebuffers...");

    framebuffers.resize(swapchainImageViews.size());

    for(size_t i = 0; i < swapchainImageViews.size(); i++) {
        vk::ImageView attachments[] = {swapchainImageViews[i]};

        vk::FramebufferCreateInfo framebufferInfo {
            {}, //flags
                renderPass,
                1,
                attachments,
                swapchainExtent.width,
                swapchainExtent.height,
                1
        };

        framebuffers[i] = device.createFramebuffer(framebufferInfo);
    }

    spdlog::info("VULKAN: Framebuffers created.");
}


/**
 * Creates the swapchain
 */
void VulkanRenderer::createSwapchain() {
    spdlog::info("VULKAN: Creating swapchain...");

    vk::SwapchainCreateInfoKHR createInfo{
        {}, //flags
            surface,
            3, //TODO: pull from config
            vk::Format::eB8G8R8A8Unorm, //TODO: pull from config
            vk::ColorSpaceKHR::eSrgbNonlinear, //TODO: pull from config
            vk::Extent2D(800, 600), //TODO: pull from config
            1, //TODO: pull from config
            vk::ImageUsageFlagBits::eColorAttachment
    };

    auto indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value()};

    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;

    createInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = vk::PresentModeKHR::eFifo; //TODO: pull from config
    createInfo.clipped = VK_TRUE;

    swapchain = device.createSwapchainKHR(createInfo);
    swapchainImages = device.getSwapchainImagesKHR(swapchain);
    swapchainImageFormat = vk::Format::eB8G8R8A8Unorm; //TODO: pull from config
    swapchainExtent = vk::Extent2D(800, 600); //TODO: pull from config

    spdlog::info("VULKAN: Swapchain created.");

    //Image views

    spdlog::info("VULKAN: Creating image views...");

    swapchainImageViews.resize(swapchainImages.size());

    for(size_t i = 0; i < swapchainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo {
            {}, //flags
                swapchainImages[i],
                vk::ImageViewType::e2D,
                swapchainImageFormat,
                {},
                {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        };

        swapchainImageViews[i] = device.createImageView(createInfo);
    }

    spdlog::info("VULKAN: Image views created.");

    //Render pass

    spdlog::info("VULKAN: Creating render pass...");

    vk::AttachmentDescription colorAttachment{
        {}, //flags
            swapchainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference colorAttachmentRef{
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::SubpassDescription subpass{
        {}, //flags
            vk::PipelineBindPoint::eGraphics,
            0,
            nullptr,
            1,
            &colorAttachmentRef
    };

    vk::RenderPassCreateInfo renderPassInfo{
        {}, //flags
            1,
            &colorAttachment,
            1,
            &subpass
    };

    renderPass = device.createRenderPass(renderPassInfo);

    spdlog::info("VULKAN: Render pass created.");
}

/**
 * Creates the command buffers
 */
void VulkanRenderer::createCommandBuffers() {
    //Command pool

    spdlog::info("VULKAN: Creating command pool...");

    auto indices = findQueueFamilies(physicalDevice);

    vk::CommandPoolCreateInfo poolInfo{
        {}, //flags
            indices.graphicsFamily.value()
    };

    commandPool = device.createCommandPool(poolInfo);

    spdlog::info("VULKAN: Command pool created.");

    //Command buffers

    spdlog::info("VULKAN: Creating command buffers...");

    commandBuffers.resize(framebuffers.size());

    vk::CommandBufferAllocateInfo allocInfo{
        commandPool,
        vk::CommandBufferLevel::ePrimary,
        (uint32_t) commandBuffers.size()
    };

    commandBuffers = device.allocateCommandBuffers(allocInfo);

    vk::ClearValue clearColor {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}};

    for(size_t i = 0; i < commandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo{
            vk::CommandBufferUsageFlagBits::eSimultaneousUse
        };

        commandBuffers[i].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassInfo{
            renderPass,
            framebuffers[i],
            {{0, 0}, swapchainExtent},
            1,
            &clearColor
        };

        commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
        commandBuffers[i].draw(3, 1, 0, 0);
        commandBuffers[i].endRenderPass();

        commandBuffers[i].end();
    }

    spdlog::info("VULKAN: Command buffers created.");

    //Semaphores and fences

    spdlog::info("VULKAN: Creating semaphores and fences...");

    imageAvailableSemaphores.resize(2);
    renderFinishedSemaphores.resize(2);
    inFlightFences.resize(2);

    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};

    for(size_t i = 0; i < 2; i++) {
        imageAvailableSemaphores[i] = device.createSemaphore(semaphoreInfo);
        renderFinishedSemaphores[i] = device.createSemaphore(semaphoreInfo);
        inFlightFences[i] = device.createFence(fenceInfo);
    }

    spdlog::info("VULKAN: Semaphores and fences created.");
}

// Class functions
VulkanRenderer::~VulkanRenderer() {
    if (instance) {
        instance.destroy();
    }
}

/**
 * Initializes the Vulkan renderer.
 */
int VulkanRenderer::init() {
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

    VkSurfaceKHR c_surface;
    SDL_bool surf_stat = SDL_Vulkan_CreateSurface(this->window, instance, &c_surface);
    if(surf_stat == SDL_FALSE) {
        std::string stat = SDL_GetError();
        spdlog::error("Failed to create Vulkan surface. {}", stat);

        return RENDERER_ERROR;
    }
    surface = vk::SurfaceKHR(c_surface);

    spdlog::info("Vulkan surface created.");

    getDevice();
    createSwapchain();
    createPipeline();
    createCommandBuffers();
}

void VulkanRenderer::render() {
    static size_t currentFrame = 0;

    device.waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = device.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE).value;

    vk::SubmitInfo submitInfo{
        1,
        &imageAvailableSemaphores[currentFrame],
        nullptr,
        1,
        &commandBuffers[imageIndex],
        1,
        &renderFinishedSemaphores[currentFrame]
    };

    device.resetFences(1, &inFlightFences[currentFrame]);

    graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame]);

    vk::PresentInfoKHR presentInfo{
        1,
        &renderFinishedSemaphores[currentFrame],
        1,
        &swapchain,
        &imageIndex
    };

    graphicsQueue.presentKHR(presentInfo);

    currentFrame = (currentFrame + 1) % 2;
}

#endif //PHOSPHOR_VULKAN