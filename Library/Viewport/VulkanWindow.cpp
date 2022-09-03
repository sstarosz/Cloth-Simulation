#include "VulkanWindow.hpp"

#include <iostream>
#include <set>
#include <string>
#include "QueueFamily.hpp"
#include "SwapChainSupport.hpp"
#include "Shader.hpp"
#include "Geometry/Vertex.hpp"

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;


[[maybe_unused]]
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger)
{
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

[[maybe_unused]]
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    VkAllocationCallbacks const* pAllocator)
{
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}


namespace st::viewport
{
	VulkanWindow::VulkanWindow()
	{
		setSurfaceType(QSurface::VulkanSurface);
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
        createFramebuffers();
        createCommandPool();
        createVertexBuffer();
        createIndexBuffer();
        /*createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();*/
	}

	void VulkanWindow::releaseResources()
	{
	}

    VkBool32 VulkanWindow::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                            void* pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    void VulkanWindow::createInstance()
	{
        //check validation layer
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layer requested, but not available!");
        }

        vk::ApplicationInfo appInfo{
                "Hello Triangle App",
                VK_MAKE_API_VERSION(1,0,0,0),
                "No Engine",
                VK_MAKE_API_VERSION(1,0,0,0),
                VK_API_VERSION_1_3
        };

        auto extensions = getRequiredExtensions();

        //To-Do check if validationLayers and extension is not empty
        vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo, validationLayers, extensions);


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
        }

        return true;
    }

    std::vector<const char*> VulkanWindow::getRequiredExtensions()
    {
        std::vector<const char*> extensions;

        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void VulkanWindow::setupDebugMessenger()
    {
        if (!enableValidationLayers) return;

        if (enableValidationLayers) {

            vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
            };

            vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags{
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
            };

            vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{ {},severityFlags,messageTypeFlags, &debugCallback };

            pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
            if (!pfnVkCreateDebugUtilsMessengerEXT)
            {
                std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
                exit(1);
            }

            pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
            if (!pfnVkDestroyDebugUtilsMessengerEXT)
            {
                std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
                exit(1);
            }

            m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
        }
    }

    void VulkanWindow::createSurface()
    {

        m_surface = QVulkanInstance::surfaceForWindow(this);

        if (!m_surface)
        {
            exit(999);
        }
    }

    void VulkanWindow::pickPhysicalDevice()
    {
        std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();

        if (devices.size() == 0)
        {
            throw std::runtime_error("Failed to find GPU's with Vulkan support!");
        }

        for (const auto& device : devices)
        {
            //Check if devices contain all required functionality
            if (isDeviceSuitable(device, m_surface))
            {
                m_physicalDevice = vk::PhysicalDevice(device);
                break;
            }
        }
    }

    void VulkanWindow::createLogicalDevice()
    {
        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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

        vk::PhysicalDeviceFeatures deviceFeatures{};
        vk::DeviceCreateInfo createInfo(
            vk::DeviceCreateFlags(),
            queueCreateInfos,
            validationLayers,
            deviceExtensions,
            &deviceFeatures
        );


        if (enableValidationLayers) {
            createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));

            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
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
        SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(m_physicalDevice, m_surface);

        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        vk::SharingMode imageSharingMode;
        auto queueFamilyIndexCount = 0;
        if (indices.graphicsFamily != indices.presentFamily) {
            imageSharingMode = vk::SharingMode::eConcurrent;

        }
        else {
            imageSharingMode = vk::SharingMode::eExclusive;
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
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && 
                availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
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

        return vk::PresentModeKHR(VK_PRESENT_MODE_FIFO_KHR);
    }

    vk::Extent2D VulkanWindow::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) 
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
        {
            return capabilities.currentExtent;
        }
        else 
        {
            int width = this->size().width();
            int height = this->size().height();

            VkExtent2D actualExtent = 
            {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanWindow::createImageViews() {
        for (const auto& swapChainImage : m_swapChainImages) {
            vk::ImageViewCreateInfo createInfo(
                vk::ImageViewCreateFlags(),
                swapChainImage,
                vk::ImageViewType::e2D,
                m_swapChainImageFormat,
                vk::ComponentMapping(vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity),
                vk::ImageSubresourceRange(vk::ImageAspectFlags(vk::ImageAspectFlagBits::eColor), 0, 1, 0, 1)
            );

            m_swapChainImageViews.emplace_back(m_device.createImageView(createInfo));
        }
    }


    void VulkanWindow::createRenderPass() {

        vk::AttachmentDescription colorAttachment(
            vk::AttachmentDescriptionFlags(),
            m_swapChainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        );

        vk::AttachmentReference colorAttachmentRef(
            0,
            vk::ImageLayout::eColorAttachmentOptimal
        );

        vk::SubpassDescription subpass(
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            {},
            colorAttachmentRef,
            {}
        );

        vk::SubpassDependency dependency(
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::AccessFlagBits::eNoneKHR,
            vk::AccessFlagBits::eColorAttachmentWrite
        );

        vk::RenderPassCreateInfo renderPassInfo(
            vk::RenderPassCreateFlags(),
            colorAttachment,
            subpass,
            dependency
        );

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

        vk::DescriptorSetLayoutCreateInfo layoutInfo{
            {},
            uboLayoutBinding
        };

        m_descriptorSetLayout = m_device.createDescriptorSetLayout(layoutInfo);
    }




    void VulkanWindow::createGraphicsPipeline() {
        auto vertShaderCode = Shader::readFile("../Assets/Shaders/vert.spv");
        auto fragShaderCode = Shader::readFile("../Assets/Shaders/frag.spv");

        vk::ShaderModule vertShaderModule = Shader::createShaderModule(m_device, vertShaderCode);
        vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo
        {
            {},
            vk::ShaderStageFlagBits::eVertex,
            vertShaderModule,
            "main"
        };

        vk::PipelineShaderStageCreateInfo fragShaderStageInfo
        {
            {},
            vk::ShaderStageFlagBits::eFragment,
            fragShaderModule,
            "main"
        };


        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{ vertShaderStageInfo, fragShaderStageInfo };


        auto bindingDescription = geometry::Vertex::getBindingDescription();
        auto attributeDescriptions = geometry::Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
            vk::PipelineVertexInputStateCreateFlags{},
            bindingDescription,
            attributeDescriptions
        );

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly(
            vk::PipelineInputAssemblyStateCreateFlags{},
            vk::PrimitiveTopology::eTriangleList,
            VK_FALSE
        );

        vk::Viewport viewport(
            0.0F,
            0.0F,
            static_cast<float>(m_swapChainExtent.width),
            static_cast<float>(m_swapChainExtent.height),
            0.0F,
            1.0F
        );


        vk::Rect2D scissor(
            vk::Offset2D{ 0, 0 },
            m_swapChainExtent
        );


        vk::PipelineViewportStateCreateInfo viewportState(
            vk::PipelineViewportStateCreateFlags{},
            viewport,
            scissor
        );

        vk::PipelineRasterizationStateCreateInfo rasterizer(
            vk::PipelineRasterizationStateCreateFlags{},
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eCounterClockwise,
            VK_FALSE,
            0.0f,
            0.0f,
            0.0f,
            1.0F
        );

        vk::PipelineMultisampleStateCreateInfo multisampling(
            vk::PipelineMultisampleStateCreateFlags{},
            vk::SampleCountFlagBits::e1,
            VK_FALSE
        );

        vk::PipelineColorBlendAttachmentState colorBlendAttachment(
            VK_FALSE,
            vk::BlendFactor::eZero,
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
            colorBlendAttachment,
            { 0.0f, 0.0f, 0.0f, 0.0f }
        );

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
            vk::PipelineLayoutCreateFlags{},
            m_descriptorSetLayout
        );

        m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

        vk::GraphicsPipelineCreateInfo pipelineInfo(
            vk::PipelineCreateFlags{},
            shaderStages,
            &vertexInputInfo,
            &inputAssembly,
            {},
            &viewportState,
            &rasterizer,
            &multisampling,
            {},
            &colorBlending,
            {},
            m_pipelineLayout,
            m_renderPass
        );

        m_pipelineCache = m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
        m_graphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;

        m_device.destroy(fragShaderModule);
        m_device.destroy(vertShaderModule);

    }

    void VulkanWindow::createFramebuffers() {
        m_swapChainFramebuffers.reserve(m_swapChainImageViews.size());

        for (const auto& attachments : m_swapChainImageViews) {
            vk::FramebufferCreateInfo framebufferInfo(
                vk::FramebufferCreateFlags{},
                m_renderPass,
                attachments,
                m_swapChainExtent.width,
                m_swapChainExtent.height,
                1
            );

            m_swapChainFramebuffers.emplace_back(m_device.createFramebuffer(framebufferInfo));
        }
    }

    void VulkanWindow::createCommandPool() {
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

    uint32_t VulkanWindow::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();


        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void VulkanWindow::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
        vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) {

        vk::BufferCreateInfo bufferInfo{
                {},
                size,
                usage,
                vk::SharingMode::eExclusive
        };

        buffer = m_device.createBuffer(bufferInfo);

        vk::MemoryRequirements memoryRequirements = m_device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo allocInfo{
                memoryRequirements.size,
                findMemoryType(memoryRequirements.memoryTypeBits, properties)
        };

        bufferMemory = m_device.allocateMemory(allocInfo);

        m_device.bindBufferMemory(buffer, bufferMemory, 0);
    }

    void VulkanWindow::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {

        vk::CommandBufferAllocateInfo allocInfo{
                m_commandPool,
                vk::CommandBufferLevel::ePrimary,
                1
        };

        auto commandBuffer = m_device.allocateCommandBuffers(allocInfo);

        vk::CommandBufferBeginInfo beginInfo{
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };

        commandBuffer.at(0).begin(beginInfo);

        vk::BufferCopy copyRegin{
            0,
            0,
            size
        };

        commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

        commandBuffer.at(0).end();

        vk::SubmitInfo submitInfo{
                {},
                {},
                commandBuffer
        };

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


        void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
        memcpy(data, indices.data(), (size_t)bufferSize);
        m_device.unmapMemory(stagingBufferMemory);

        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            m_indexBuffer,
            m_indexBufferMemory);

        copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);
        m_device.destroyBuffer(stagingBuffer);
        m_device.freeMemory(stagingBufferMemory);
    }

} //!namespace st::viewport