#include "VulkanWindow.hpp"

#include "Geometry/Vertex.hpp"
#include "Line.hpp"
#include "QueueFamily.hpp"
#include "Shader.hpp"
#include "SwapChainSupport.hpp"
#include <iostream>
#include <set>
#include <string>
#include <QMouseEvent>
#include <QKeyEvent>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "IO/ImporterProxy.hpp"

#include <span>

namespace st::viewport {

VulkanWindow::VulkanWindow()
{
    setSurfaceType(QSurface::VulkanSurface);
}

void VulkanWindow::initialize()
{

    m_renderer.createInstance();
	createInstance();

	auto m_surface = static_cast<vk::SurfaceKHR>(QVulkanInstance::surfaceForWindow(this));
	if (!m_surface)
	{
		exit(999);
	}

	m_renderer.setupSurface(m_surface);



	m_renderer.initialize();




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
    createUniformBuffers();
	loadModel();
    createVertexBuffer();
    createIndexBuffer();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();

    isInitialised = true;
}

void VulkanWindow::releaseResources()
{
    cleanupSwapChain();

    m_device.destroyPipelineCache(m_pipelineCache);
    m_device.destroyPipeline(m_graphicsPipeline);
    m_device.destroyPipelineLayout(m_pipelineLayout);
    m_device.destroyRenderPass(m_renderPass);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_device.destroyBuffer(m_uniformBuffers[i]);
        m_device.freeMemory(m_uniformBuffersMemory[i]);
    }

    m_device.destroyDescriptorPool(m_descriptorPool);

    m_device.destroySampler(m_textureSampler);
    m_device.destroyImageView(m_textureImageView);

    m_device.destroyImage(m_textureImage);
    m_device.freeMemory(m_textureImageMemory);

    m_device.destroyDescriptorSetLayout(m_descriptorSetLayout);

    m_device.destroyBuffer(m_indexBuffer);
    m_device.freeMemory(m_indexBufferMemory);

    m_device.destroyBuffer(m_vertexBuffer);
    m_device.freeMemory(m_vertexBufferMemory);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_device.destroySemaphore(m_renderFinishedSemaphores[i]);
        m_device.destroySemaphore(m_imageAvailableSemaphores[i]);
        m_device.destroyFence(m_inFlightFences[i]);
    }

    m_device.destroyCommandPool(m_commandPool);

    m_device.destroy();

    
    m_renderer.releaseResources();
}

void VulkanWindow::createInstance()
{
	inst.setVkInstance(m_renderer.getInstance());

    if (!inst.create())
    {
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    }

    setVulkanInstance(&inst);
}

void VulkanWindow::pickPhysicalDevice()
{
	std::vector<vk::PhysicalDevice> devices
		= m_renderer.getInstance().enumeratePhysicalDevices();

    if (devices.empty()) {
        throw std::runtime_error("Failed to find GPU's with Vulkan support!");
    }

    for (const auto& device : devices) {
        // Check if devices contain all required functionality
        if (isDeviceSuitable(device, m_renderer.getSurface())) {
            m_physicalDevice = vk::PhysicalDevice(device);
            break;
        }
    }
}

void VulkanWindow::createLogicalDevice()
{
	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(
		m_physicalDevice, m_renderer.getSurface()
	);
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0F;
    for (const auto& queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
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
	SwapChainSupportDetails swapChainSupport
		= SwapChainSupportDetails::querySwapChainSupport(
			m_physicalDevice, m_renderer.getSurface()
		);

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(
		m_physicalDevice, m_renderer.getSurface()
	);
    std::array<uint32_t, 2> queueFamilyIndices{indices.graphicsFamily.value(), indices.presentFamily.value()};

    vk::SharingMode imageSharingMode;
    if (indices.graphicsFamily != indices.presentFamily) {
        imageSharingMode = vk::SharingMode::eConcurrent;
    } else {
        imageSharingMode = vk::SharingMode::eExclusive;
    }

    vk::SwapchainCreateInfoKHR createInfo(
		vk::SwapchainCreateFlagsKHR(), m_renderer.getSurface(),
                                            imageCount,
                                            surfaceFormat.format,
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

    return vk::PresentModeKHR(VK_PRESENT_MODE_FIFO_KHR);
}

vk::Extent2D VulkanWindow::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width = this->size().width();
        int height = this->size().height();

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
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

        m_swapChainImageViews.emplace_back(m_device.createImageView(createInfo));
    }
}

void VulkanWindow::createRenderPass()
{

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

    vk::AttachmentDescription depthAttachment {
		vk::AttachmentDescriptionFlags(),
		findDepthFormat(),
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};


    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);


    vk::SubpassDescription subpass(
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        {},
		colorAttachmentRef,
        {},
        &depthAttachmentRef
	);

    vk::SubpassDependency dependency(
        VK_SUBPASS_EXTERNAL,
        0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
        vk::AccessFlagBits::eNoneKHR,
        vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);



    std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    vk::RenderPassCreateInfo renderPassInfo(
        vk::RenderPassCreateFlags(),
        attachments,
        subpass,
        dependency);

    m_renderPass = m_device.createRenderPass(renderPassInfo);
}

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

    m_descriptorSetLayout = m_device.createDescriptorSetLayout(layoutInfo);
}

void VulkanWindow::createGraphicsPipeline()
{
    auto vertShaderCode = Shader::readFile("../Assets/Shaders/vert.spv");
    auto fragShaderCode = Shader::readFile("../Assets/Shaders/frag.spv");

    vk::ShaderModule vertShaderModule = Shader::createShaderModule(m_device, vertShaderCode);
    vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo {
        {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main"
    };

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo {
        {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main"
    };

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };

    auto bindingDescription = geometry::Vertex::getBindingDescription();
    auto attributeDescriptions = geometry::Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        {},
        bindingDescription,
        attributeDescriptions
    );

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly(
        vk::PipelineInputAssemblyStateCreateFlags {},
        vk::PrimitiveTopology::eTriangleList,
        VK_FALSE);

    //vk::Viewport viewport(
    //    0.0F,
    //    0.0F,
    //    static_cast<float>(m_swapChainExtent.width),
    //    static_cast<float>(m_swapChainExtent.height),
    //    0.0F,
    //    1.0F);
    //
    //vk::Rect2D scissor(
    //    vk::Offset2D { 0, 0 },
    //    m_swapChainExtent);

    vk::PipelineViewportStateCreateInfo viewportState(
        vk::PipelineViewportStateCreateFlags {},
        1,
        {},
        1,
        {}
        );

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

    m_dynamicStateEnables = {vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    m_pipelineDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{ {}, m_dynamicStateEnables};

    vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        VK_FALSE,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    vk::PipelineColorBlendStateCreateInfo colorBlending(
        vk::PipelineColorBlendStateCreateFlags {},
        VK_FALSE,
        vk::LogicOp::eCopy,
        colorBlendAttachment,
        { 0.0f, 0.0f, 0.0f, 0.0f });

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
        vk::PipelineLayoutCreateFlags {},
        m_descriptorSetLayout);

    m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

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
        &m_pipelineDynamicStateCreateInfo,
        m_pipelineLayout,
        m_renderPass);

    m_pipelineCache = m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
    m_graphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;


    //Note VkShaderModule is passed into pipline and are not longer available trought object they are used to create
    //If ther are used later, then they must not be destroyed
    m_device.destroy(fragShaderModule);
    m_device.destroy(vertShaderModule);
}

void VulkanWindow::createFramebuffers()
{
    m_swapChainFramebuffers.reserve(m_swapChainImageViews.size());

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

        m_swapChainFramebuffers.emplace_back(m_device.createFramebuffer(framebufferInfo));
    }
}

void VulkanWindow::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::findQueueFamilies(
		m_physicalDevice, m_renderer.getSurface()
	);

    //TODO This is a graphic commandPoll
    vk::CommandPoolCreateInfo poolInfo(
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        queueFamilyIndices.graphicsFamily.value()
    );

    m_commandPool = m_device.createCommandPool(poolInfo);
}

void VulkanWindow::loadModel()
{

    //TODO Change from load vertices and indices
    //to models and objects
	io::ImporterProxy importerProxy;
    importerProxy.readFile("../Assets/Models/Cube.obj");
	vertices = importerProxy.getVertices();
    m_indices = importerProxy.getIndices();

    m_line.createDescriptorSets(m_device, m_uniformBuffers);
    m_line.createPrimitivePipline(m_device, m_swapChainExtent, m_renderPass);
    m_line.createLineVertexBuffer(m_physicalDevice, m_device, m_commandPool, m_graphicsQueue);
    m_line.createLineIndexBuffer(m_physicalDevice, m_device, m_commandPool, m_graphicsQueue);
}

void VulkanWindow::createVertexBuffer()
{
    //vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    //vk::Buffer stagingBuffer;
    //vk::DeviceMemory stagingBufferMemory;

    //createBuffer(bufferSize,
    //    vk::BufferUsageFlagBits::eTransferSrc,
    //    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    //    stagingBuffer,
    //    stagingBufferMemory
    //);

    //void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
    //memcpy(data, vertices.data(), (size_t)bufferSize); //vertices should fullfil trival object specyfication?
    //m_device.unmapMemory(stagingBufferMemory);

    //createBuffer(bufferSize,
    //    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
    //    vk::MemoryPropertyFlagBits::eDeviceLocal,
    //    m_vertexBuffer,
    //    m_vertexBufferMemory);

    //copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);
    //m_device.destroyBuffer(stagingBuffer);
    //m_device.freeMemory(stagingBufferMemory);



    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    static bool isBufferInitialized = false;

    if (!isBufferInitialized)
	{
		createBuffer(
			bufferSize, vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible
				| vk::MemoryPropertyFlagBits::eHostCoherent,
			m_vertexBuffer, m_vertexBufferMemory
		);

        isBufferInitialized = true;
    }


    std::span<std::byte> data {static_cast<std::byte*>(m_device.mapMemory(m_vertexBufferMemory, 0, bufferSize)),bufferSize};
	std::memcpy(data.data(), vertices.data(), static_cast<size_t>(bufferSize)); //vertices should fullfil trival object specyfication?
	m_device.unmapMemory(m_vertexBufferMemory);
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

void VulkanWindow::createBuffer(vk::DeviceSize size,
                            vk::BufferUsageFlags usage,
                            vk::MemoryPropertyFlags properties,
                            vk::Buffer& buffer,
                            vk::DeviceMemory& bufferMemory)
{

    vk::BufferCreateInfo bufferInfo { {}, size, usage, vk::SharingMode::eExclusive };

    buffer = m_device.createBuffer(bufferInfo);

    vk::MemoryRequirements memoryRequirements = m_device.getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo { memoryRequirements.size,
                                       findMemoryType(memoryRequirements.memoryTypeBits, properties)};

    bufferMemory = m_device.allocateMemory(allocInfo);

    m_device.bindBufferMemory(buffer, bufferMemory, 0);
}

void VulkanWindow::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{

    vk::CommandBufferAllocateInfo allocInfo { m_commandPool, vk::CommandBufferLevel::ePrimary, 1 };

    auto commandBuffer = m_device.allocateCommandBuffers(allocInfo);

    vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    commandBuffer.at(0).begin(beginInfo);

    vk::BufferCopy copyRegin { 0, 0, size };

    commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

    commandBuffer.at(0).end();

    vk::SubmitInfo submitInfo { {}, {}, commandBuffer };

    m_graphicsQueue.submit(1, &submitInfo, {}); //TODO GraphicQueue should copy buffers?
    m_graphicsQueue.waitIdle();

    m_device.freeCommandBuffers(m_commandPool, commandBuffer);
}

void VulkanWindow::createIndexBuffer()
{
    vk::DeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer,
        stagingBufferMemory);

    void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, m_indices.data(), (size_t)bufferSize);
    m_device.unmapMemory(stagingBufferMemory);

    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_indexBuffer,
        m_indexBufferMemory);

    copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    m_device.destroyBuffer(stagingBuffer);
    m_device.freeMemory(stagingBufferMemory);
}

void VulkanWindow::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    m_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
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

void VulkanWindow::createCommandBuffers()
{
    vk::CommandBufferAllocateInfo allocInfo(m_commandPool, vk::CommandBufferLevel::ePrimary,
        static_cast<uint32_t>(m_swapChainFramebuffers.size()));

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


    UniformBufferObject ubo{};
    ubo.model = geometry::Matrix4x4::indentityMatrix();
    ubo.model.convertToColumnMajor();
       
    ubo.view = m_camera.getViewMatrix();
    ubo.view.convertToColumnMajor();
       
    ubo.proj = m_camera.getProjectionMatrix(45.0F, m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1F, 100.0F);
    ubo.proj.convertToColumnMajor();


    void* data = m_device.mapMemory(m_uniformBuffersMemory[currentImage], 0, sizeof(ubo));
    memcpy(data, &ubo, sizeof(ubo));
    m_device.unmapMemory(m_uniformBuffersMemory[currentImage]);
}

void VulkanWindow::recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
{

    commandBuffer.begin(vk::CommandBufferBeginInfo {});

    vk::ClearColorValue colorClean{ std::array<float, 4>{0.0F, 0.0F, 0.0F, 1.0F} };
    vk::ClearDepthStencilValue depthClean{ 1.0F, 0 };

    std::array<vk::ClearValue, 2> clearValues;
	clearValues[0].setColor(colorClean);
    clearValues[1].setDepthStencil(depthClean);


    //Draw primitive
    vk::RenderPassBeginInfo renderPassInfo(m_renderPass,
                                           m_swapChainFramebuffers[imageIndex],
                                           vk::Rect2D((0, 0),
                                           m_swapChainExtent),
                                           clearValues);

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    vk::Viewport viewport{
        0.0F,
        0.0F,
        static_cast<float>(m_swapChainExtent.width),
        static_cast<float>(m_swapChainExtent.height),
        0.0F,
        1.0F
    };

    vk::Rect2D scissor{ {0, 0}, m_swapChainExtent };
    commandBuffer.setViewport(0, 1, &viewport);
    commandBuffer.setScissor(0, 1, &scissor);


    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);

    vk::Buffer vertexBuffers[] = { m_vertexBuffer };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    commandBuffer.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint32);

    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0,
        m_descriptorSets[currentFrame], {});

    commandBuffer.drawIndexed(static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);


    //Line
    
    //Bind Line pipeline
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_line.getPipeline());
    // 
    //Bind Vertex Buffer from line
    commandBuffer.bindVertexBuffers(0, m_line.getVertexBuffer(), m_line.getVertexBufferOffsets());
    commandBuffer.bindIndexBuffer(m_line.getIndexBuffer(), 0, vk::IndexType::eUint32);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_line.getPiplineLayout(), 0, m_line.getDescritporSet(currentFrame), {});
    commandBuffer.drawIndexed(m_line.getIndexSize(), 1, 0, 0, 0);
    //Bind IndexBuffer form Line
    //Bind Descriptiors?
    //Draw Indexed
    





    commandBuffer.endRenderPass();
    // End Draw primitive


    commandBuffer.end();


}

void VulkanWindow::drawFrame()
{
    auto resultFence = m_device.waitForFences(m_inFlightFences.at(currentFrame), VK_TRUE, UINT64_MAX);
    if (resultFence != vk::Result::eSuccess) {
        std::cout << "syf" << std::endl;
    }

    auto [result, imageIndex] = m_device.acquireNextImageKHR(m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);
	
    updateGeometry();
    updateUniformBuffer(currentFrame);

    m_device.resetFences(m_inFlightFences.at(currentFrame));

    m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlags {});
    recordCommandBuffer(m_commandBuffers[currentFrame], imageIndex);

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

    vk::SubmitInfo submitInfo(m_imageAvailableSemaphores[currentFrame],
                              waitDestinationStageMask,
                              m_commandBuffers[currentFrame],
                              m_renderFinishedSemaphores[currentFrame]);

    m_graphicsQueue.submit(submitInfo, m_inFlightFences[currentFrame]);

    vk::PresentInfoKHR presentInfo(m_renderFinishedSemaphores[currentFrame],
                                   m_swapChain,
                                   imageIndex);

    try {
        result = m_presentQueue.presentKHR(presentInfo);
    } catch (std::exception const& exc) {
        std::cerr << exc.what();
        recreateSwapChain();
    }

    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_framebufferResized) {
        m_framebufferResized = false;
        recreateSwapChain();
    } else if (result != vk::Result::eSuccess) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    requestUpdate();
}

void VulkanWindow::updateGeometry()
{
	static float time { 0.0F };


    const float amplitude = 0.01F;	
    const float frequency = 0.1F;
	const float moveX = amplitude * std::cos(frequency * time);
	std::cout << moveX << std::endl;

    for (auto& vertex : vertices)
	{
		vertex.m_pos.x += moveX;
    }


    time += 0.1F;
    createVertexBuffer();
}


void VulkanWindow::resizeEvent(QResizeEvent* event)
{
    if (isInitialised)
    {
        recreateSwapChain();
    }
    QWindow::resizeEvent(event);
}

bool VulkanWindow::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        update();
        break;

    case QEvent::PlatformSurface:
        // When windows is created or deleted
        // d->releaseSwapChain();
        //d->reset();
        break;

    default:
        break;
    }

    return QWindow::event(event);
}

void VulkanWindow::mousePressEvent(QMouseEvent* event)
{
    // left alt + left   mouse click + move -> rotate
    // left alt + middle mouse click + move -> pan
    // left alt + right  mouse click + move up and down -> zoom
    auto m_fromClick = event->position().toPoint();

    //record start and stop
    if (event->modifiers() & Qt::AltModifier)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            m_camera.mousePressEvent(m_fromClick.x(), m_fromClick.y(),Camera::Actions::Orbit);
            qDebug() << "Left (Orbit) Start: " << m_fromClick << "\n";

        }

        if (event->buttons() & Qt::MiddleButton)
        {
            m_camera.mousePressEvent(m_fromClick.x(), m_fromClick.y(), Camera::Actions::Pan);
            qDebug() << "Middle (Pan) Start:" << m_fromClick << "\n";

        }

        if (event->buttons() & Qt::RightButton)
        {
            m_camera.mousePressEvent(m_fromClick.x(), m_fromClick.y(), Camera::Actions::Zoom);
            qDebug() << "Right (Zoom) Start:" << m_fromClick << "\n";

        }
    }

    event->accept();
}

void VulkanWindow::mouseMoveEvent(QMouseEvent* event)
{
    const auto mousePosition = event->position().toPoint();
    m_camera.mouseMove(mousePosition.x(), mousePosition.y());
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent* event)
{
    const auto mousePosition = event->position().toPoint();
    m_camera.mouseMove(mousePosition.x(), mousePosition.y());
    m_camera.releaseMouseClick();
}

void VulkanWindow::keyPressEvent(QKeyEvent* event)
{

}

void VulkanWindow::keyReleaseEvent(QKeyEvent* event)
{

}

void VulkanWindow::exposeEvent(QExposeEvent* /*unused*/)
{
    requestUpdate();
}

void VulkanWindow::cleanupSwapChain()
{

    m_device.destroyImageView(m_depthImageView);
    m_device.destroyImage(m_depthImage);
    m_device.freeMemory(m_depthImageMemory);

    for (auto framebuffer : m_swapChainFramebuffers) {
        m_device.destroy(framebuffer);
    }
    m_swapChainFramebuffers.clear();


    for (auto imageView : m_swapChainImageViews) {
        m_device.destroy(imageView);
    }
    m_swapChainImageViews.clear();

    m_device.destroySwapchainKHR(m_swapChain);
}

void VulkanWindow::recreateSwapChain()
{
    if (isInitialised)
    {
        m_device.waitIdle();

        cleanupSwapChain();
        createSwapChain();
        createImageViews();
        createDepthResources();
        createFramebuffers();
    }
}

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

    stbi_uc* pixels = stbi_load("../Assets/Textures/texture2.jpg", &texWidth,
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
        false,
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

    //transitionImageLayout(m_depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
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
