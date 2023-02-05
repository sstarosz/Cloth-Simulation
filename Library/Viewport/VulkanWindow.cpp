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

VulkanWindow::VulkanWindow():
	m_instance(std::make_unique<renderer::StInstance>()),
	m_surface(),
	m_renderer()
{
    setSurfaceType(QSurface::VulkanSurface);
}

void VulkanWindow::initialize()
{


	m_instance->create();
	createQtInstance(m_instance->getInstance());
	auto surface = static_cast<vk::SurfaceKHR>(QVulkanInstance::surfaceForWindow(this));
	if (!surface)
	{
		exit(999);
	}


	m_surface = std::make_unique<renderer::Surface>(surface);
	m_renderer = std::make_unique<renderer::Renderer>(*m_instance, *m_surface);


    m_renderer->updateSwapChain(static_cast<uint64_t>(this->size().width()), static_cast<uint64_t>(this->size().height()));
	m_renderer->initialize();



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

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
         m_renderer->getLogicalDevice().destroyBuffer(m_uniformBuffers[i]);
         m_renderer->getLogicalDevice().freeMemory(m_uniformBuffersMemory[i]);
    }

    m_renderer->getLogicalDevice().destroyDescriptorPool(m_descriptorPool);

    m_renderer->getLogicalDevice().destroySampler(m_textureSampler);
    m_renderer->getLogicalDevice().destroyImageView(m_textureImageView);

    m_renderer->getLogicalDevice().destroyImage(m_textureImage);
    m_renderer->getLogicalDevice().freeMemory(m_textureImageMemory);

    m_renderer->getLogicalDevice().destroyBuffer(m_indexBuffer);
	m_renderer->getLogicalDevice().freeMemory(m_indexBufferMemory);

    m_renderer->getLogicalDevice().destroyBuffer(m_vertexBuffer);
	m_renderer->getLogicalDevice().freeMemory(m_vertexBufferMemory);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		m_renderer->getLogicalDevice().destroySemaphore(m_renderFinishedSemaphores[i]);
		m_renderer->getLogicalDevice().destroySemaphore(m_imageAvailableSemaphores[i]);
		m_renderer->getLogicalDevice().destroyFence(m_inFlightFences[i]);
    }

  
    m_renderer->releaseResources();
}

void VulkanWindow::createQtInstance(vk::Instance instance)
{
	inst.setVkInstance(instance);

    if (!inst.create())
    {
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    }

    setVulkanInstance(&inst);
}

void VulkanWindow::createFramebuffers()
{
	auto swapchainImageViews = m_renderer->getSwapChainImagesViews();
	m_swapChainFramebuffers.reserve(swapchainImageViews.size());

    for (const auto& swapChainImageView : swapchainImageViews)
	{
        std::array<vk::ImageView, 2> attachments = {
            swapChainImageView,
            m_depthImageView
        };

        vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags {},
            m_renderer->getRenderPass(),
            attachments,
			m_renderer->getSwapchainExtend2D().width,
			m_renderer->getSwapchainExtend2D().height,
            1);

        m_swapChainFramebuffers.emplace_back(
			m_renderer->getLogicalDevice().createFramebuffer(framebufferInfo)
		);
    }
}

void VulkanWindow::loadModel()
{


    //TODO Change from load vertices and indices
    //to models and objects
	io::ImporterProxy importerProxy;
    importerProxy.readFile("../Assets/Models/Cube.obj");
	vertices = importerProxy.getVertices();
    m_indices = importerProxy.getIndices();

    vk::Extent2D swapChainExtent = m_renderer->getSwapchainExtend2D();



    m_line.createDescriptorSets(m_renderer->getLogicalDevice(), m_uniformBuffers);
	m_line.createPrimitivePipline(
		m_renderer->getLogicalDevice(), swapChainExtent, m_renderer->getRenderPass()
	);
	m_line.createLineVertexBuffer(
		m_renderer->getPhysicalDevice(), m_renderer->getLogicalDevice(),
		m_renderer->getCommandPool(),
		m_renderer->getGraphicsQueue()
	);
	m_line.createLineIndexBuffer(
		m_renderer->getPhysicalDevice(), m_renderer->getLogicalDevice(),
		m_renderer->getCommandPool(),
		m_renderer->getGraphicsQueue()
	);
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


    std::span<std::byte> data {
		static_cast<std::byte*>(
			m_renderer->getLogicalDevice().mapMemory(m_vertexBufferMemory, 0, bufferSize)
		),
		bufferSize
	};
	std::memcpy(data.data(), vertices.data(), static_cast<size_t>(bufferSize)); //vertices should fullfil trival object specyfication?
	m_renderer->getLogicalDevice().unmapMemory(m_vertexBufferMemory);
}

uint32_t VulkanWindow::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
	vk::PhysicalDeviceMemoryProperties memProperties
		= m_renderer->getPhysicalDevice().getMemoryProperties();

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

    buffer = m_renderer->getLogicalDevice().createBuffer(bufferInfo);

    vk::MemoryRequirements memoryRequirements
		= m_renderer->getLogicalDevice().getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo { memoryRequirements.size,
                                       findMemoryType(memoryRequirements.memoryTypeBits, properties)};

    bufferMemory = m_renderer->getLogicalDevice().allocateMemory(allocInfo);

    m_renderer->getLogicalDevice().bindBufferMemory(buffer, bufferMemory, 0);
}

void VulkanWindow::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{

    vk::CommandBufferAllocateInfo allocInfo { m_renderer->getCommandPool(),
											  vk::CommandBufferLevel::ePrimary, 1 };

    auto commandBuffer = m_renderer->getLogicalDevice().allocateCommandBuffers(allocInfo);

    vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    commandBuffer.at(0).begin(beginInfo);

    vk::BufferCopy copyRegin { 0, 0, size };

    commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

    commandBuffer.at(0).end();

    vk::SubmitInfo submitInfo { {}, {}, commandBuffer };

    m_renderer->getGraphicsQueue().submit(
		1, &submitInfo, {}
	); //TODO GraphicQueue should copy buffers?
	m_renderer->getGraphicsQueue().waitIdle();

    m_renderer->getLogicalDevice().freeCommandBuffers(
		m_renderer->getCommandPool(), commandBuffer
	);
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

    void* data
		= m_renderer->getLogicalDevice().mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, m_indices.data(), (size_t)bufferSize);
	m_renderer->getLogicalDevice().unmapMemory(stagingBufferMemory);

    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_indexBuffer,
        m_indexBufferMemory);

    copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    m_renderer->getLogicalDevice().destroyBuffer(stagingBuffer);
	m_renderer->getLogicalDevice().freeMemory(stagingBufferMemory);
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

    m_descriptorPool = m_renderer->getLogicalDevice().createDescriptorPool(poolInfo);
}

void VulkanWindow::createCommandBuffers()
{
	vk::CommandBufferAllocateInfo allocInfo(
		m_renderer->getCommandPool(), vk::CommandBufferLevel::ePrimary,
        static_cast<uint32_t>(m_swapChainFramebuffers.size()));

    m_commandBuffers = m_renderer->getLogicalDevice().allocateCommandBuffers(allocInfo);
}

void VulkanWindow::createSyncObjects()
{
    m_imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		m_imageAvailableSemaphores.emplace_back(
			m_renderer->getLogicalDevice().createSemaphore(vk::SemaphoreCreateInfo {})
		);
		m_renderFinishedSemaphores.emplace_back(
			m_renderer->getLogicalDevice().createSemaphore(vk::SemaphoreCreateInfo {})
		);
		m_inFlightFences.emplace_back(m_renderer->getLogicalDevice().createFence(
			vk::FenceCreateInfo { vk::FenceCreateFlagBits::eSignaled }
		));
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
       
    ubo.proj = m_camera.getProjectionMatrix(
		45.0F,
		(m_renderer->getSwapchainExtend2D().width/ static_cast<float>(m_renderer->getSwapchainExtend2D().height)),
		0.1F, 100.0F);
    ubo.proj.convertToColumnMajor();


    void* data = m_renderer->getLogicalDevice().mapMemory(
		m_uniformBuffersMemory[currentImage], 0, sizeof(ubo)
	);
    memcpy(data, &ubo, sizeof(ubo));
	m_renderer->getLogicalDevice().unmapMemory(m_uniformBuffersMemory[currentImage]);
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
	vk::Extent2D swapChainExtent = m_renderer->getSwapchainExtend2D();
	vk::RenderPassBeginInfo renderPassInfo(
		m_renderer->getRenderPass(),
                                           m_swapChainFramebuffers[imageIndex],
                                           vk::Rect2D((0, 0), swapChainExtent),
                                           clearValues);

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    vk::Viewport viewport{
        0.0F,
        0.0F,
		static_cast<float>(swapChainExtent.width),
		static_cast<float>(swapChainExtent.height),
        0.0F,
        1.0F
    };

    vk::Rect2D scissor {
		{0, 0},
        swapChainExtent
	};
    commandBuffer.setViewport(0, 1, &viewport);
    commandBuffer.setScissor(0, 1, &scissor);


    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_renderer->getGraphicsPipeline());

    vk::Buffer vertexBuffers[] = { m_vertexBuffer };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    commandBuffer.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint32);

    commandBuffer.bindDescriptorSets(
		vk::PipelineBindPoint::eGraphics,
        m_renderer->getPipelineLayout(),
        0,
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
	auto resultFence = m_renderer->getLogicalDevice().waitForFences(
		m_inFlightFences.at(currentFrame), VK_TRUE, UINT64_MAX
	);
    if (resultFence != vk::Result::eSuccess) {
        std::cout << "syf" << std::endl;
    }

    auto [result, imageIndex] = m_renderer->getLogicalDevice().acquireNextImageKHR(
		m_renderer->getSwapchain(), UINT64_MAX, m_imageAvailableSemaphores[currentFrame],
		VK_NULL_HANDLE
	);
	
    updateGeometry();
    updateUniformBuffer(currentFrame);

    m_renderer->getLogicalDevice().resetFences(m_inFlightFences.at(currentFrame));

    m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlags {});
    recordCommandBuffer(m_commandBuffers[currentFrame], imageIndex);

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

    vk::SubmitInfo submitInfo(m_imageAvailableSemaphores[currentFrame],
                              waitDestinationStageMask,
                              m_commandBuffers[currentFrame],
                              m_renderFinishedSemaphores[currentFrame]);

    m_renderer->getGraphicsQueue().submit(submitInfo, m_inFlightFences[currentFrame]);

    vk::PresentInfoKHR presentInfo(m_renderFinishedSemaphores[currentFrame], m_renderer->getSwapchain(),
                                   imageIndex);

    try {
		result = m_renderer->getPresentationQueue().presentKHR(presentInfo);
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

    m_renderer->getLogicalDevice().destroyImageView(m_depthImageView);
    m_renderer->getLogicalDevice().destroyImage(m_depthImage);
    m_renderer->getLogicalDevice().freeMemory(m_depthImageMemory);

    for (auto& framebuffer : m_swapChainFramebuffers) {
		m_renderer->getLogicalDevice().destroy(framebuffer);
    }
    m_swapChainFramebuffers.clear();


}

void VulkanWindow::recreateSwapChain()
{
    if (isInitialised)
    {
		m_renderer->getLogicalDevice().waitIdle();

        cleanupSwapChain();
		m_renderer->recreateSwapChain(
			static_cast<uint64_t>(this->size().width()),
			static_cast<uint64_t>(this->size().height())
		);
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
    std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_renderer->getDescriptorSetLayout());
    vk::DescriptorSetAllocateInfo allocInfo {
        m_descriptorPool,
        layouts
    };

    m_descriptorSets = m_renderer->getLogicalDevice().allocateDescriptorSets(allocInfo);

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

        m_renderer->getLogicalDevice().updateDescriptorSets(descriptorWrites, {});
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

    void* data
		= m_renderer->getLogicalDevice().mapMemory(stagingBufferMemory, 0, imageSize);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
	m_renderer->getLogicalDevice().unmapMemory(stagingBufferMemory);

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

    m_renderer->getLogicalDevice().destroyBuffer(stagingBuffer);
    m_renderer->getLogicalDevice().freeMemory(stagingBufferMemory);
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

    image = m_renderer->getLogicalDevice().createImage(imageInfo);

    vk::MemoryRequirements m_memRequirements
		= m_renderer->getLogicalDevice().getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo allocInfo { m_memRequirements.size,
        findMemoryType(m_memRequirements.memoryTypeBits, properties) };

    imageMemory = m_renderer->getLogicalDevice().allocateMemory(allocInfo);

    m_renderer->getLogicalDevice().bindImageMemory(image, imageMemory, 0);
}

vk::CommandBuffer VulkanWindow::beginSingleTimeCommands()
{

    vk::CommandBufferAllocateInfo allocInfo { m_renderer->getCommandPool(),
        vk::CommandBufferLevel::ePrimary,
        1
    };

    vk::CommandBuffer commandBuffer
		= m_renderer->getLogicalDevice().allocateCommandBuffers(allocInfo).front();
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

    m_renderer->getGraphicsQueue().submit(submitInfo);
	m_renderer->getGraphicsQueue().waitIdle();

    m_renderer->getLogicalDevice().freeCommandBuffers(
		m_renderer->getCommandPool(), commandBuffer
	);
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

    return m_renderer->getLogicalDevice().createImageView(viewInfo);
}

void VulkanWindow::createTextureSampler()
{

    vk::PhysicalDeviceProperties properties
		= m_renderer->getPhysicalDevice().getProperties();

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

    m_textureSampler = m_renderer->getLogicalDevice().createSampler(sampleInfo);
}

void VulkanWindow::createDepthResources()
{
    vk::Format depthFormat = findDepthFormat();

    vk::Extent2D swapChainExtent = m_renderer->getSwapchainExtend2D();

    createImage(
		        swapChainExtent.width,
                swapChainExtent.height,
                depthFormat,
                vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eDepthStencilAttachment,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                m_depthImage,
                m_depthImageMemory);

    m_depthImageView = createImageView(m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);

}

vk::Format VulkanWindow::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
    vk::FormatFeatureFlags features)
{
    for (const auto& format : candidates) {
		vk::FormatProperties props {
			m_renderer->getPhysicalDevice().getFormatProperties(format)
		};

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
