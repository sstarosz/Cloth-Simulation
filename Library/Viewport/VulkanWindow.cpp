#include "VulkanWindow.hpp"

#include "Geometry/Vertex.hpp"
#include "QueueFamily.hpp"
#include "Shader.hpp"
#include "SwapChainSupport.hpp"
#include <iostream>
#include <set>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL
vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    VkAllocationCallbacks const* pAllocator)
{
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

namespace st::viewport {

VulkanWindow::VulkanWindow()
{
    setSurfaceType(QSurface::VulkanSurface);
}
	}
void VulkanWindow::initialize()
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createDepthResources();
    createFramebuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}
	}

void VulkanWindow::releaseResources()
{
}

VkBool32 VulkanWindow::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void VulkanWindow::createInstance()
{
    // check validation layer
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layer requested, but not available!");
    }
    //TODO replace String to reading from config file?
    vk::ApplicationInfo appInfo { "Hello Triangle App", VK_MAKE_API_VERSION(1, 0, 0, 0), "No Engine",
        VK_MAKE_API_VERSION(1U, 0U, 0U, 0U), VK_API_VERSION_1_3 };
                VK_MAKE_API_VERSION(1,0,0,0),
                "No Engine",
                VK_MAKE_API_VERSION(1,0,0,0),
                VK_API_VERSION_1_3
        };

    auto extensions = getRequiredExtensions();

    // To-Do check if validationLayers and extension is not empty
    if (!inst.create())
    {
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    }
    m_instance = vk::createInstance(instanceCreateInfo);
    inst.setVkInstance(m_instance);

        if (!inst.create())
            qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    setVulkanInstance(&inst);
}

bool VulkanWindow::checkValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const auto* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
std::vector<const char*> VulkanWindow::getRequiredExtensions() const
{
    std::vector<const char*> extensions;
}

    std::vector<const char*> VulkanWindow::getRequiredExtensions()
    {
        std::vector<const char*> extensions;

    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
void VulkanWindow::setupDebugMessenger()
{
    if (!enableValidationLayers)
    {
        return;
    }
}

    void VulkanWindow::setupDebugMessenger()
    {
        if (!enableValidationLayers) return;

    if (enableValidationLayers) {

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags { vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError };

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags { vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation };

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT {
            {}, severityFlags, messageTypeFlags, &debugCallback
        };

        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!pfnVkCreateDebugUtilsMessengerEXT) {
            std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
            exit(1);
        }

        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (!pfnVkDestroyDebugUtilsMessengerEXT) {
            std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function."
                      << std::endl;
            exit(1);
        }

        m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }
}

void VulkanWindow::createSurface()
{

    m_surface = QVulkanInstance::surfaceForWindow(this);

    if (!m_surface) {
        exit(999);
    }
}
    if (devices.empty()) {
        throw std::runtime_error("Failed to find GPU's with Vulkan support!");
    }
    std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();

        if (devices.size() == 0)
        {
            throw std::runtime_error("Failed to find GPU's with Vulkan support!");
        }

    for (const auto& device : devices) {
        // Check if devices contain all required functionality
        if (isDeviceSuitable(device, m_surface)) {
            m_physicalDevice = vk::PhysicalDevice(device);
            break;
        }
    }
}

void VulkanWindow::createLogicalDevice()
    float queuePriority = 1.0F;
    for (const auto& queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
        queueCreateInfos.push_back(deviceQueueCreateInfo);
    }

        float queuePriority = 1.0f;
        for (const auto& queueFamily : uniqueQueueFamilies) {
            vk::DeviceQueueCreateInfo deviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                queueFamily,
                1,
                &queuePriority
            );
            queueCreateInfos.push_back(deviceQueueCreateInfo);
        }

    vk::PhysicalDeviceFeatures deviceFeatures {};
    vk::DeviceCreateInfo createInfo(vk::DeviceCreateFlags(), queueCreateInfos, validationLayers, deviceExtensions,
        &deviceFeatures);

    if (enableValidationLayers) {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    m_device = m_physicalDevice.createDevice(createInfo);

    m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
    m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
}

bool VulkanWindow::isDeviceSuitable(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
    QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool VulkanWindow::checkDeviceExtensionSupport(const vk::PhysicalDevice& device)
{

    const std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanWindow::createSwapChain()
{
    QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);
    std::array<uint32_t, 2> queueFamilyIndices{indices.graphicsFamily.value(), indices.presentFamily.value()};
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::SharingMode imageSharingMode;
    if (indices.graphicsFamily != indices.presentFamily) {
        imageSharingMode = vk::SharingMode::eConcurrent;
    } else {
        imageSharingMode = vk::SharingMode::eExclusive;
    }

    vk::SwapchainCreateInfoKHR createInfo(vk::SwapchainCreateFlagsKHR(), m_surface, imageCount, surfaceFormat.format,
                                            surfaceFormat.colorSpace,
                                            extent,
                                            1,
                                            vk::ImageUsageFlagBits::eColorAttachment,
                                            imageSharingMode,
                                            queueFamilyIndices,
                                            swapChainSupport.capabilities.currentTransform,
                                            vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                            presentMode,
                                            VK_TRUE, VK_NULL_HANDLE);
        }

        vk::SwapchainCreateInfoKHR createInfo(
            vk::SwapchainCreateFlagsKHR(),
            m_surface,
            imageCount,
            surfaceFormat.format,
            surfaceFormat.colorSpace,
            extent,
            1,
            vk::ImageUsageFlagBits::eColorAttachment,
            imageSharingMode,
            queueFamilyIndexCount,
            queueFamilyIndices,
            swapChainSupport.capabilities.currentTransform,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            presentMode,
            VK_TRUE,
            VK_NULL_HANDLE
        );

    m_swapChain = m_device.createSwapchainKHR(createInfo);
    m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

vk::SurfaceFormatKHR VulkanWindow::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR VulkanWindow::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }

void VulkanWindow::createImageViews()
{
    for (const auto& swapChainImage : m_swapChainImages) {
        vk::ImageViewCreateInfo createInfo(
            vk::ImageViewCreateFlags(),
            swapChainImage,
            vk::ImageViewType::e2D,
            m_swapChainImageFormat,
            vk::ComponentMapping(vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity),
            vk::ImageSubresourceRange(vk::ImageAspectFlags(vk::ImageAspectFlagBits::eColor), 0, 1, 0, 1));

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
    vk::AttachmentDescription colorAttachment(
        vk::AttachmentDescriptionFlags(),
        m_swapChainImageFormat,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR);
                vk::ImageSubresourceRange(vk::ImageAspectFlags(vk::ImageAspectFlagBits::eColor), 0, 1, 0, 1)
            );

        m_swapChainImageViews.emplace_back(m_device.createImageView(createInfo));
    }
}

void VulkanWindow::createRenderPass()
{

    void VulkanWindow::createRenderPass() {

        vk::AttachmentDescription colorAttachment(
            vk::AttachmentDescriptionFlags(),
            m_swapChainImageFormat,
void VulkanWindow::createDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding uboLayoutBinding {
        0,
        vk::DescriptorType::eUniformBuffer,
        1,
        vk::ShaderStageFlagBits::eVertex
    };

    vk::DescriptorSetLayoutBinding samplerLayoutBinding {
        1,
        vk::DescriptorType::eCombinedImageSampler,
        1,
        vk::ShaderStageFlagBits::eFragment
    };

    std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    vk::DescriptorSetLayoutCreateInfo layoutInfo {
        {},
        bindings
    };

    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eNoneKHR,
        vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(), colorAttachment, subpass, dependency);

    m_renderPass = m_device.createRenderPass(renderPassInfo);
}

    void VulkanWindow::createDescriptorSetLayout()
    {
        vk::DescriptorSetLayoutBinding uboLayoutBinding
        {
            0,
            vk::DescriptorType::eUniformBuffer,
            1,
            vk::ShaderStageFlagBits::eVertex
        };

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };

    auto bindingDescription = geometry::Vertex::getBindingDescription();
    auto attributeDescriptions = geometry::Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        {},
        bindingDescription,
        attributeDescriptions);
{
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly(
        vk::PipelineInputAssemblyStateCreateFlags {},
        vk::PrimitiveTopology::eTriangleList,
        VK_FALSE);
    vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);
    vk::Viewport viewport(
        0.0F,
        0.0F,
        static_cast<float>(m_swapChainExtent.width),
        static_cast<float>(m_swapChainExtent.height),
        0.0F,
        1.0F);
    };
    vk::Rect2D scissor(
        vk::Offset2D { 0, 0 },
        m_swapChainExtent);

    vk::PipelineViewportStateCreateInfo viewportState(
        vk::PipelineViewportStateCreateFlags {},
        viewport,
        scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizer(
        vk::PipelineRasterizationStateCreateFlags {},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eCounterClockwise,
        VK_FALSE,
        0.0f,
        0.0f,
        0.0f,
        1.0F);
            0.0F,
    vk::PipelineMultisampleStateCreateInfo multisampling(
        vk::PipelineMultisampleStateCreateFlags {},
        vk::SampleCountFlagBits::e1,
        VK_FALSE);

    vk::PipelineDepthStencilStateCreateInfo depthStencil {
        {},
        true,
        true,
        vk::CompareOp::eLess,
        false,
        false,
    };
        );
    vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        VK_FALSE,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
            vk::PipelineViewportStateCreateFlags{},
    vk::PipelineColorBlendStateCreateInfo colorBlending(
        vk::PipelineColorBlendStateCreateFlags {},
        VK_FALSE,
        vk::LogicOp::eCopy,
        colorBlendAttachment,
        { 0.0f, 0.0f, 0.0f, 0.0f });
            VK_FALSE,
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
        vk::PipelineLayoutCreateFlags {},
        m_descriptorSetLayout);
            vk::FrontFace::eCounterClockwise,
            VK_FALSE,
            0.0f,
    vk::GraphicsPipelineCreateInfo pipelineInfo(
        vk::PipelineCreateFlags {},
        shaderStages,
        &vertexInputInfo,
        &inputAssembly,
        {},
        &viewportState,
        &rasterizer,
        &multisampling,
        &depthStencil,
        &colorBlending,
        {},
        m_pipelineLayout,
        m_renderPass);
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eZero,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        );

        vk::PipelineColorBlendStateCreateInfo colorBlending(
            vk::PipelineColorBlendStateCreateFlags{},
            VK_FALSE,
            vk::LogicOp::eCopy,
    for (const auto& swapChainImageView : m_swapChainImageViews) {
        std::array<vk::ImageView, 2> attachments = {
            swapChainImageView,
            m_depthImageView
        };

        vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags {},
            m_renderPass,
            attachments,
            m_swapChainExtent.width,
            m_swapChainExtent.height,
            1);
    m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

        vk::GraphicsPipelineCreateInfo pipelineInfo(
            vk::PipelineCreateFlags{},
            shaderStages,
            &vertexInputInfo,
            &inputAssembly,
            {},
            &viewportState,
    vk::CommandPoolCreateInfo poolInfo(
        vk::CommandPoolCreateFlags { vk::CommandPoolCreateFlagBits::eResetCommandBuffer },
        queueFamilyIndices.graphicsFamily.value()
    );
            {},
            m_pipelineLayout,
            m_renderPass
        );
void VulkanWindow::loadModel()
{

    //TODO load to vertices
    // and load to m_indices

    //m_object;

}

void VulkanWindow::createVertexBuffer()
{
    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    m_graphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;

    m_device.destroy(fragShaderModule);
    m_device.destroy(vertShaderModule);
    createBuffer(bufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer,
        stagingBufferMemory
    );
    m_swapChainFramebuffers.reserve(m_swapChainImageViews.size());

        for (const auto& attachments : m_swapChainImageViews) {
            vk::FramebufferCreateInfo framebufferInfo(
                vk::FramebufferCreateFlags{},
    createBuffer(bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_vertexBuffer,
        m_vertexBufferMemory);
                1
    copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);
    m_device.destroyBuffer(stagingBuffer);
    m_device.freeMemory(stagingBufferMemory);
}


void VulkanWindow::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);

        vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags{ vk::CommandPoolCreateFlagBits::eResetCommandBuffer },
            queueFamilyIndices.graphicsFamily.value()
        );

    m_commandPool = m_device.createCommandPool(poolInfo);
}

    void VulkanWindow::createVertexBuffer() {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;

        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer, stagingBufferMemory);


    void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    m_device.unmapMemory(stagingBufferMemory);


        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            m_vertexBuffer, m_vertexBufferMemory);

        copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);
        m_device.destroyBuffer(stagingBuffer);
        m_device.freeMemory(stagingBufferMemory);

    }

uint32_t VulkanWindow::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanWindow::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
    vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
{

    vk::BufferCreateInfo bufferInfo { {}, size, usage, vk::SharingMode::eExclusive };
void VulkanWindow::createIndexBuffer()
{
    vk::DeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();
    vk::MemoryRequirements memoryRequirements = m_device.getBufferMemoryRequirements(buffer);
    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer,
        stagingBufferMemory);
}
    void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, m_indices.data(), (size_t)bufferSize);
    m_device.unmapMemory(stagingBufferMemory);

    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_indexBuffer,
        m_indexBufferMemory);
    vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
    copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    m_device.destroyBuffer(stagingBuffer);
    m_device.freeMemory(stagingBufferMemory);
}

    commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

    commandBuffer.at(0).end();

    vk::SubmitInfo submitInfo { {}, {}, commandBuffer };

    m_graphicsQueue.submit(1, &submitInfo, {});
    m_graphicsQueue.waitIdle();

    m_device.freeCommandBuffers(m_commandPool, commandBuffer);
}


    void VulkanWindow::createIndexBuffer() {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        createBuffer(bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer,
            stagingBufferMemory);
void VulkanWindow::createCommandBuffers()
{
    vk::CommandBufferAllocateInfo allocInfo(m_commandPool, vk::CommandBufferLevel::ePrimary,
        static_cast<uint32_t>(m_swapChainFramebuffers.size()));
            m_indexBufferMemory);
    m_commandBuffers = m_device.allocateCommandBuffers(allocInfo);
}
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            m_uniformBuffers[i], m_uniformBuffersMemory[i]);
    }
}

void VulkanWindow::createDescriptorPool()
{
    vk::DescriptorPoolSize poolSize { vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) };

    vk::DescriptorPoolCreateInfo poolInfo { {}, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), poolSize };

    m_descriptorPool = m_device.createDescriptorPool(poolInfo);
}

    void VulkanWindow::createDescriptorSets()
    {
        std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocInfo
        {
            m_descriptorPool,
            layouts
    UniformBufferObject ubo {};
    ubo.model = glm::rotate(glm::mat4(1.0F), time * glm::radians(90.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    ubo.view = glm::lookAt(glm::vec3(2.0F, 2.0F, 2.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, 1.0F));
    ubo.proj = glm::perspective(glm::radians(45.0F), m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1F, 10.0F);
    ubo.proj[1][1] *= -1;
        {
            vk::DescriptorBufferInfo bufferInfo{
                m_uniformBuffers.at(i),
                0,
                sizeof(UniformBufferObject)
            };

            vk::WriteDescriptorSet descriptorWrite{
                m_descriptorSets.at(i),
                0,
                0,
    vk::ClearValue clearValues;
    clearValues.color = vk::ClearColorValue(std::array<float, 4>({ { 0.0F, 0.0F, 0.0F, 1.0F } }));
                bufferInfo,
    vk::RenderPassBeginInfo renderPassInfo(m_renderPass, m_swapChainFramebuffers[imageIndex],
        vk::Rect2D((0, 0), m_swapChainExtent), clearValues);

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        }

    }

    void VulkanWindow::createCommandBuffers() {
        vk::CommandBufferAllocateInfo allocInfo(
            m_commandPool,
            vk::CommandBufferLevel::ePrimary,
            static_cast<uint32_t>(m_swapChainFramebuffers.size())
        );
    commandBuffer.drawIndexed(static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
        m_commandBuffers = m_device.allocateCommandBuffers(allocInfo);

    }

void VulkanWindow::createSyncObjects()
{
    m_imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_imageAvailableSemaphores.emplace_back(m_device.createSemaphore(vk::SemaphoreCreateInfo {}));
        m_renderFinishedSemaphores.emplace_back(m_device.createSemaphore(vk::SemaphoreCreateInfo {}));
        m_inFlightFences.emplace_back(m_device.createFence(vk::FenceCreateInfo { vk::FenceCreateFlagBits::eSignaled }));
    }
}

void VulkanWindow::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    vk::SubmitInfo submitInfo(m_imageAvailableSemaphores[currentFrame], waitDestinationStageMask,
        m_commandBuffers[currentFrame], m_renderFinishedSemaphores[currentFrame]);

    m_graphicsQueue.submit(submitInfo, m_inFlightFences[currentFrame]);
        ubo.proj[1][1] *= -1;

    void* data = m_device.mapMemory(m_uniformBuffersMemory[currentImage], 0, sizeof(ubo));
    memcpy(data, &ubo, sizeof(ubo));
    m_device.unmapMemory(m_uniformBuffersMemory[currentImage]);
}

void VulkanWindow::recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
{

    commandBuffer.begin(vk::CommandBufferBeginInfo {});

        vk::ClearValue clearValues;
        clearValues.color = vk::ClearColorValue(std::array<float, 4>({ { 0.0f, 0.0f, 0.0f, 1.0f } }));

        vk::RenderPassBeginInfo renderPassInfo(
            m_renderPass,
            m_swapChainFramebuffers[imageIndex],
            vk::Rect2D((0, 0), m_swapChainExtent),
            clearValues
        );


        commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);
bool VulkanWindow::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        update();
        break;

    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0,
        m_descriptorSets[currentFrame], {});

        commandBuffer.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    commandBuffer.endRenderPass();

    return QWindow::event(event);
}

void VulkanWindow::exposeEvent(QExposeEvent* /*unused*/)
{
    requestUpdate();
}
    if (resultFence != vk::Result::eSuccess) {
        std::cout << "syf" << std::endl;
    }

    auto [result, imageIndex] = m_device.acquireNextImageKHR(m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);

    updateUniformBuffer(currentFrame);

    m_device.resetFences(m_inFlightFences.at(currentFrame));

    m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlags {});
    recordCommandBuffer(m_commandBuffers[currentFrame], imageIndex);

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

        vk::SubmitInfo submitInfo(
            m_imageAvailableSemaphores[currentFrame],
            waitDestinationStageMask,
            m_commandBuffers[currentFrame],
            m_renderFinishedSemaphores[currentFrame]
        );


        m_graphicsQueue.submit(submitInfo, m_inFlightFences[currentFrame]);

    vk::PresentInfoKHR presentInfo(m_renderFinishedSemaphores[currentFrame], m_swapChain, imageIndex);

    try {
        result = m_presentQueue.presentKHR(presentInfo);
    } catch (std::exception const& exc) {
        std::cerr << exc.what();
        recreateSwapChain();
    }

    if (/*result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||*/ m_framebufferResized) {
        m_framebufferResized = false;
void VulkanWindow::update()
{
    drawFrame();
}

void VulkanWindow::createDescriptorSets()
{
    std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);
    vk::DescriptorSetAllocateInfo allocInfo {
        m_descriptorPool,
        layouts
    };

    m_descriptorSets = m_device.allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vk::DescriptorBufferInfo bufferInfo {
            m_uniformBuffers.at(i),
            0,
            sizeof(UniformBufferObject)
        };

        vk::DescriptorImageInfo imageInfo {
            m_textureSampler,
            m_textureImageView,
            vk::ImageLayout::eShaderReadOnlyOptimal
        };

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites {
            vk::WriteDescriptorSet {
                m_descriptorSets.at(i),
                0,
                0,
                vk::DescriptorType::eUniformBuffer,
                {},
                bufferInfo,
                {} },
            vk::WriteDescriptorSet {
                m_descriptorSets.at(i),
                1,
                0,
                vk::DescriptorType::eCombinedImageSampler,
                imageInfo,
                {},
                {} }
        };

        m_device.updateDescriptorSets(descriptorWrites, {});
    }
}

void VulkanWindow::createTextureImage()
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;

    stbi_uc* pixels = stbi_load("../Assets/Textures/texture.jpg", &texWidth,
        &texHeight, &texChannels, STBI_rgb_alpha);

    vk::DeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("Failed to load texture image!");
    }

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;

    createBuffer(imageSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer, stagingBufferMemory);

    void* data = m_device.mapMemory(stagingBufferMemory, 0, imageSize);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    m_device.unmapMemory(stagingBufferMemory);

    stbi_image_free(pixels);

    createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
        vk::MemoryPropertyFlagBits::eDeviceLocal, m_textureImage, m_textureImageMemory);

    transitionImageLayout(m_textureImage,
        vk::Format::eR8G8B8A8Srgb,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal);
    copyBufferToImage(
        stagingBuffer,
        m_textureImage,
        static_cast<uint32_t>(texWidth),
        static_cast<uint32_t>(texHeight));

    transitionImageLayout(m_textureImage,
        vk::Format::eR8G8B8A8Srgb,
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::eShaderReadOnlyOptimal);

    m_device.destroyBuffer(stagingBuffer);
    m_device.freeMemory(stagingBufferMemory);
}

void VulkanWindow::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
    vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image,
    vk::DeviceMemory& imageMemory)
{

    vk::ImageCreateInfo imageInfo { {},
        vk::ImageType::e2D,
        format,
        { width, height, 1 },
        1,
        1,
        vk::SampleCountFlagBits::e1,
        tiling,
        usage,
        vk::SharingMode::eExclusive,
        {},
        vk::ImageLayout::eUndefined };

    image = m_device.createImage(imageInfo);

    vk::MemoryRequirements m_memRequirements = m_device.getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo allocInfo { m_memRequirements.size,
        findMemoryType(m_memRequirements.memoryTypeBits, properties) };

    imageMemory = m_device.allocateMemory(allocInfo);

    m_device.bindImageMemory(image, imageMemory, 0);
}

vk::CommandBuffer VulkanWindow::beginSingleTimeCommands()
{

    vk::CommandBufferAllocateInfo allocInfo {
        m_commandPool,
        vk::CommandBufferLevel::ePrimary,
        1
    };

    vk::CommandBuffer commandBuffer = m_device.allocateCommandBuffers(allocInfo).front();
    vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    commandBuffer.begin(beginInfo);

    return commandBuffer;
}

void VulkanWindow::endSingleTimeCommands(vk::CommandBuffer commandBuffer)
{
    commandBuffer.end();

    vk::SubmitInfo submitInfo {
        {},
        {},
        commandBuffer
    };

    m_graphicsQueue.submit(submitInfo);
    m_graphicsQueue.waitIdle();

    m_device.freeCommandBuffers(m_commandPool, commandBuffer);
}

void VulkanWindow::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout)
{
    vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::ImageMemoryBarrier barrier {
        vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eNone,
        oldLayout,
        newLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        image,
        { vk::ImageAspectFlagBits::eColor,
            0,
            1,
            0,
            1 }
    };

    vk::PipelineStageFlags sourceStage {};
    vk::PipelineStageFlags destinationStage {};

    if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
        barrier.srcAccessMask = vk::AccessFlagBits::eNone;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
    } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    } else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
        barrier.srcAccessMask = vk::AccessFlagBits::eNone;
        barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    commandBuffer.pipelineBarrier(
        sourceStage, destinationStage,
        {},
        {},
        {},
        barrier);

    endSingleTimeCommands(commandBuffer);
}

void VulkanWindow::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
{

    vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::BufferImageCopy region {
        0,
        0,
        0,
        { vk::ImageAspectFlagBits::eColor,
            0,
            0,
            1 },
        { 0,
            0,
            0 },
        { width,
            height,
            1 }
    };

    commandBuffer.copyBufferToImage(
        buffer,
        image,
        vk::ImageLayout::eTransferDstOptimal,
        region);

    endSingleTimeCommands(commandBuffer);
}

void VulkanWindow::createTextureImageView()
{

    m_textureImageView = createImageView(m_textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
}

vk::ImageView VulkanWindow::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{

    vk::ImageViewCreateInfo viewInfo {
        {},
        image,
        vk::ImageViewType::e2D,
        format,
        {},
        { aspectFlags,
            0,
            1,
            0,
            1 },
        {}
    };

    return m_device.createImageView(viewInfo);
}

void VulkanWindow::createTextureSampler()
{

    vk::PhysicalDeviceProperties properties = m_physicalDevice.getProperties();

    vk::SamplerCreateInfo sampleInfo {
        {},
        vk::Filter::eLinear,
        vk::Filter::eLinear,
        vk::SamplerMipmapMode::eLinear,
        vk::SamplerAddressMode::eRepeat,
        vk::SamplerAddressMode::eRepeat,
        vk::SamplerAddressMode::eRepeat,
        0.0f,
        true,
        properties.limits.maxSamplerAnisotropy,
        false,
        vk::CompareOp::eAlways,
        0.0f,
        0.0f,
        vk::BorderColor::eIntOpaqueBlack,
        false,
    };

    m_textureSampler = m_device.createSampler(sampleInfo);
}

void VulkanWindow::createDepthResources()
{
    vk::Format depthFormat = findDepthFormat();

    createImage(m_swapChainExtent.width,
        m_swapChainExtent.height,
        depthFormat,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eDepthStencilAttachment,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_depthImage,
        m_depthImageMemory);

    m_depthImageView = createImageView(m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);

    transitionImageLayout(m_depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
}

vk::Format VulkanWindow::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
    vk::FormatFeatureFlags features)
{
    for (const auto& format : candidates) {
        vk::FormatProperties props { m_physicalDevice.getFormatProperties(format) };

        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

vk::Format VulkanWindow::findDepthFormat()
{
    return findSupportedFormat(
        { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}




} // namespace st::viewport
    requestUpdate();
}

void VulkanWindow::resizeEvent(QResizeEvent*)
{
    // QWindow::resizeEvent( < unnamed >);
}

    bool VulkanWindow::event(QEvent* ev) {
        switch (ev->type()) {
        case QEvent::UpdateRequest:
            update();
            break;

    case QEvent::PlatformSurface:
        break;

    default:
        break;
    }

        return QWindow::event(ev);
    }

    void VulkanWindow::exposeEvent(QExposeEvent*) {
        requestUpdate();
    }

void VulkanWindow::cleanupSwapChain()
{
    for (auto framebuffer : m_swapChainFramebuffers) {
        m_device.destroy(framebuffer);
    }

    m_swapChainFramebuffers.clear();

    m_device.destroy(m_pipelineCache);
    m_device.destroy(m_graphicsPipeline);
    m_device.destroy(m_pipelineLayout);
    m_device.destroy(m_renderPass);

    for (auto imageView : m_swapChainImageViews) {
        m_device.destroy(imageView);
    }
    m_swapChainImageViews.clear();

    m_device.destroy(m_swapChain);
}

void VulkanWindow::recreateSwapChain()
{
    m_device.waitIdle();

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandBuffers();
}


    void VulkanWindow::update()
    {
        drawFrame();
    }
} //!namespace st::viewport