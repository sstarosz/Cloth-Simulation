#include "Renderer.hpp"
#include "ValidationLayers.hpp"
#include "Extensions.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <chrono>

namespace st::renderer
{

	Renderer::Renderer(const StInstance& instance, const Surface& surface):
		m_instance(instance),
		m_surface(surface),
		m_physicalDevice(m_instance.getInstance(), surface.getSurface()),
		m_logicalDevice(m_instance.getInstance(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
		m_swapChain(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), m_logicalDevice.getDevice()),
		m_renderPass(m_physicalDevice.getPhysicalDevice(), m_swapChain.getSwapChainImageFormat(), m_logicalDevice.getDevice()),
		m_graphicPipeline(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_renderPass.getRenderPass()),
		m_primitivesGraphicPipeline(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_renderPass.getRenderPass()),
		m_commandPool(m_logicalDevice.getDevice(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
		m_memoryManager(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_commandPool.getCommandPool(), m_logicalDevice.getGraphiceQueue()),
		m_imageManager(m_logicalDevice.getDevice(), m_commandPool.getCommandPool(), m_logicalDevice.getGraphiceQueue(), m_memoryManager),
		m_framebuffer(m_logicalDevice.getDevice(), m_swapChain, m_renderPass, m_imageManager)
	{ }


	void Renderer::initialize()
	{
		m_physicalDevice.initialize();
		m_logicalDevice.initialize();
		m_swapChain.initialize();
		m_renderPass.initialize();
		m_graphicPipeline.initialize();
		m_primitivesGraphicPipeline.initialize();
		m_commandPool.initialize();
		m_framebuffer.initialize();


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
	}

	void Renderer::releaseResources()
	{
		m_logicalDevice.getDevice().waitIdle();
		m_swapChain.releaseResources();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_logicalDevice.getDevice().destroyBuffer(m_uniformBuffers[i]);
			m_logicalDevice.getDevice().freeMemory(m_uniformBuffersMemory[i]);
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_logicalDevice.getDevice().destroySemaphore(m_renderFinishedSemaphores[i]);
			m_logicalDevice.getDevice().destroySemaphore(m_imageAvailableSemaphores[i]);
			m_logicalDevice.getDevice().destroyFence(m_inFlightFences[i]);
		}

		m_logicalDevice.getDevice().destroyDescriptorPool(m_descriptorPool);
		m_logicalDevice.getDevice().destroyDescriptorPool(m_lineDescriptorPool);


		m_logicalDevice.getDevice().destroySampler(m_textureSampler);
		m_logicalDevice.getDevice().destroyImageView(m_textureImageView);

		m_logicalDevice.getDevice().destroyImage(m_textureImage);
		m_logicalDevice.getDevice().freeMemory(m_textureImageMemory);

		m_logicalDevice.getDevice().destroyBuffer(m_indexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_indexBufferMemory);
		m_logicalDevice.getDevice().destroyBuffer(m_lineIndexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_lineIndexBufferMemory);

		m_logicalDevice.getDevice().destroyBuffer(m_vertexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_vertexBufferMemory);
		m_logicalDevice.getDevice().destroyBuffer(m_lineVertexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_lineVertexBufferMemory);

		//Reverse order then initialization
		m_framebuffer.releaseResources();
		m_commandPool.releaseResources();
		m_graphicPipeline.releaseResources();
		m_primitivesGraphicPipeline.releaseResources();
		m_renderPass.releaseResources();
		m_logicalDevice.releaseResources();
		m_physicalDevice.releaseResources();
	}


	//void Renderer::mainLoop()
	//{

	//while(end)?
	//read event?
	//events from qt as signals so is not needed?

	//simulate
	//render

	//}

	void Renderer::renderFrame()
	{

		auto resultFence = m_logicalDevice.getDevice().waitForFences(m_inFlightFences.at(currentFrame), VK_TRUE, UINT64_MAX);
		if (resultFence != vk::Result::eSuccess)
		{
			//std::cout << "syf" << std::endl;
		}

		auto [result, imageIndex] =
			m_logicalDevice.getDevice().acquireNextImageKHR(m_swapChain.getSwapchain(), UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);

		updateGeometry();
		updateUniformBuffer(currentFrame);


		m_logicalDevice.getDevice().resetFences(m_inFlightFences.at(currentFrame));

		m_commandBuffers[currentFrame].reset(vk::CommandBufferResetFlags {});
		recordCommandBuffer(m_commandBuffers[currentFrame], imageIndex);


		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

		vk::SubmitInfo submitInfo(m_imageAvailableSemaphores[currentFrame],
								  waitDestinationStageMask,
								  m_commandBuffers[currentFrame],
								  m_renderFinishedSemaphores[currentFrame]);


		m_logicalDevice.getGraphiceQueue().submit(submitInfo, m_inFlightFences[currentFrame]);


		vk::PresentInfoKHR presentInfo(m_renderFinishedSemaphores[currentFrame], m_swapChain.getSwapchain(), imageIndex);

		try
		{
			result = m_logicalDevice.getPresentQueue().presentKHR(presentInfo);
		}
		catch (std::exception const& exc)
		{
			std::cerr << exc.what();
			//TODO - Fix
			//recreateSwapChain();
		}

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_framebufferResized)
		{
			m_framebufferResized = false;
			//TODO - Fix
			//recreateSwapChain();
		}
		else if (result != vk::Result::eSuccess)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	vk::Instance Renderer::getInstance() const
	{
		return m_instance.getInstance();
	}

	void Renderer::updateSwapChain(uint64_t width, uint64_t height)
	{
		m_swapChain.updateSwapChain(width, height);
	}

	void Renderer::recreateSwapChain(uint64_t width, uint64_t height)
	{
		m_logicalDevice.getDevice().waitIdle();
		m_framebuffer.releaseResources();
		m_swapChain.recreateSwapChain(width, height);
		m_framebuffer.initialize();
	}

	//TO-DO remove it
	static bool isBufferInitialized = false;

	void Renderer::createVertexBuffer()
	{

		vk::DeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
		//vk::DeviceSize lineBufferSize = sizeof(m_lines[0]) * m_lines.size();


		if (!isBufferInitialized)
		{

			//TODO - Allocate more space
			m_memoryManager.createBuffer(bufferSize,
										 vk::BufferUsageFlagBits::eVertexBuffer,
										 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
										 m_vertexBuffer,
										 m_vertexBufferMemory);


			//Line
			vk::DeviceSize lineBufferSize = sizeof(m_lines[0]) * m_lines.size();

			vk::Buffer lineStagingBuffer;
			vk::DeviceMemory lineStagingBufferMemory;


			m_memoryManager.createBuffer(lineBufferSize,
										 vk::BufferUsageFlagBits::eTransferSrc,
										 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
										 lineStagingBuffer,
										 lineStagingBufferMemory);

			void* lineData = m_logicalDevice.getDevice().mapMemory(lineStagingBufferMemory, 0, lineBufferSize);
			memcpy(lineData, m_lines.data(), (size_t)lineBufferSize); //vertices should fullfil trival object specyfication?
			m_logicalDevice.getDevice().unmapMemory(lineStagingBufferMemory);

			m_memoryManager.createBuffer(lineBufferSize,
										 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
										 vk::MemoryPropertyFlagBits::eDeviceLocal,
										 m_lineVertexBuffer,
										 m_lineVertexBufferMemory);

			m_memoryManager.copyBuffer(lineStagingBuffer, m_lineVertexBuffer, lineBufferSize);

			m_logicalDevice.getDevice().destroyBuffer(lineStagingBuffer);
			m_logicalDevice.getDevice().freeMemory(lineStagingBufferMemory);



			isBufferInitialized = true;
		}


		std::span<std::byte> data { static_cast<std::byte*>(m_logicalDevice.getDevice().mapMemory(m_vertexBufferMemory, 0, bufferSize)), bufferSize };
		//std::span<std::byte> lineData { static_cast<std::byte*>(m_logicalDevice.getDevice().mapMemory(m_lineVertexBufferMemory, 0, lineBufferSize)),lineBufferSize};

		//TODO multiple memcpy per object
		std::memcpy(data.data(), m_vertices.data(),
					static_cast<size_t>(bufferSize)); //vertices should fulfill trivial object specification?

		m_logicalDevice.getDevice().unmapMemory(m_vertexBufferMemory);
		//m_logicalDevice.getDevice().unmapMemory(m_lineVertexBufferMemory);



	}

	void Renderer::createIndexBuffer()
	{
		vk::DeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		m_memoryManager.createBuffer(bufferSize,
									 vk::BufferUsageFlagBits::eTransferSrc,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 stagingBuffer,
									 stagingBufferMemory);

		void* data = m_logicalDevice.getDevice().mapMemory(stagingBufferMemory, 0, bufferSize);
		memcpy(data, m_indices.data(), (size_t)bufferSize);
		m_logicalDevice.getDevice().unmapMemory(stagingBufferMemory);

		m_memoryManager.createBuffer(bufferSize,
									 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
									 vk::MemoryPropertyFlagBits::eDeviceLocal,
									 m_indexBuffer,
									 m_indexBufferMemory);

		m_memoryManager.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

		m_logicalDevice.getDevice().destroyBuffer(stagingBuffer);
		m_logicalDevice.getDevice().freeMemory(stagingBufferMemory);


		//---------------------------------Line-------------------------------------------------------
		vk::DeviceSize lineBufferSize = sizeof(m_linesIndices[0]) * m_linesIndices.size();

		vk::Buffer lineStagingBuffer;
		vk::DeviceMemory lineStagingBufferMemory;
		m_memoryManager.createBuffer(lineBufferSize,
									 vk::BufferUsageFlagBits::eTransferSrc,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 lineStagingBuffer,
									 lineStagingBufferMemory);

		void* lineData = m_logicalDevice.getDevice().mapMemory(lineStagingBufferMemory, 0, lineBufferSize);
		memcpy(lineData, m_linesIndices.data(), (size_t)lineBufferSize);
		m_logicalDevice.getDevice().unmapMemory(lineStagingBufferMemory);

		m_memoryManager.createBuffer(lineBufferSize,
									 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
									 vk::MemoryPropertyFlagBits::eDeviceLocal,
									 m_lineIndexBuffer,
									 m_lineIndexBufferMemory);

		m_memoryManager.copyBuffer(lineStagingBuffer, m_lineIndexBuffer, lineBufferSize);

		m_logicalDevice.getDevice().destroyBuffer(lineStagingBuffer);
		m_logicalDevice.getDevice().freeMemory(lineStagingBufferMemory);
	}

	void Renderer::createUniformBuffers()
	{

		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_memoryManager.createBuffer(bufferSize,
										 vk::BufferUsageFlagBits::eUniformBuffer,
										 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
										 m_uniformBuffers[i],
										 m_uniformBuffersMemory[i]);
		}
	}

	void Renderer::createCommandBuffers()
	{
		vk::CommandBufferAllocateInfo allocInfo { m_commandPool.getCommandPool(),
												  vk::CommandBufferLevel::ePrimary,
												  static_cast<uint32_t>(m_framebuffer.getSwapchainFramebuffersBuffers().size()) };

		m_commandBuffers = m_logicalDevice.getDevice().allocateCommandBuffers(allocInfo);
	}

	void Renderer::updateUniformBuffer(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();


		UniformBufferObject ubo {};
		ubo.model = geometry::Matrix4x4::indentityMatrix();
		ubo.model.convertToColumnMajor();

		ubo.view = m_camera.getViewMatrix();
		ubo.view.convertToColumnMajor();

		ubo.proj = m_camera.getProjectionMatrix(45.0F,
												(m_swapChain.getSwapchainExtend2D().width / static_cast<float>(m_swapChain.getSwapchainExtend2D().height)),
												0.1F,
												100.0F);
		ubo.proj.convertToColumnMajor();


		void* data = m_logicalDevice.getDevice().mapMemory(m_uniformBuffersMemory[currentImage], 0, sizeof(ubo));
		memcpy(data, &ubo, sizeof(ubo));
		m_logicalDevice.getDevice().unmapMemory(m_uniformBuffersMemory[currentImage]);
	}

	void Renderer::createSyncObjects()
	{
		m_imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_imageAvailableSemaphores.emplace_back(m_logicalDevice.getDevice().createSemaphore(vk::SemaphoreCreateInfo {}));
			m_renderFinishedSemaphores.emplace_back(m_logicalDevice.getDevice().createSemaphore(vk::SemaphoreCreateInfo {}));
			m_inFlightFences.emplace_back(m_logicalDevice.getDevice().createFence(vk::FenceCreateInfo { vk::FenceCreateFlagBits::eSignaled }));
		}
	}

	void Renderer::createDescriptorPool()
	{
		vk::DescriptorPoolSize poolSize { vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) };

		vk::DescriptorPoolCreateInfo poolInfo { {}, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), poolSize };

		m_descriptorPool = m_logicalDevice.getDevice().createDescriptorPool(poolInfo);


		//Create Line Pool
		vk::DescriptorPoolSize linePoolSize { vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) };

		vk::DescriptorPoolCreateInfo linePoolInfo { {}, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), linePoolSize };

		m_lineDescriptorPool = m_logicalDevice.getDevice().createDescriptorPool(linePoolInfo);
	}

	void Renderer::createDescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> graphicLayouts(MAX_FRAMES_IN_FLIGHT, m_graphicPipeline.getDescriptorSetLayout());
		std::vector<vk::DescriptorSetLayout> primitiveLayouts(MAX_FRAMES_IN_FLIGHT, m_primitivesGraphicPipeline.getDescriptorSetLayout());

		vk::DescriptorSetAllocateInfo graphicAllocInfo { m_descriptorPool, graphicLayouts };
		vk::DescriptorSetAllocateInfo primitiveAllocInfo { m_lineDescriptorPool, primitiveLayouts };


		m_descriptorSets = m_logicalDevice.getDevice().allocateDescriptorSets(graphicAllocInfo);
		m_lineDescriptorSets = m_logicalDevice.getDevice().allocateDescriptorSets(primitiveAllocInfo);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vk::DescriptorBufferInfo bufferInfo { m_uniformBuffers.at(i), 0, sizeof(UniformBufferObject) };


			vk::DescriptorImageInfo imageInfo { m_textureSampler, m_textureImageView, vk::ImageLayout::eShaderReadOnlyOptimal };

			std::array<vk::WriteDescriptorSet, 2> graphicDescriptorWrites {
				vk::WriteDescriptorSet {m_descriptorSets.at(i),  0, 0, vk::DescriptorType::eUniformBuffer,        {},        bufferInfo, {}},
				vk::WriteDescriptorSet { m_descriptorSets.at(i), 1, 0, vk::DescriptorType::eCombinedImageSampler, imageInfo, {},         {}}
			};

			std::array<vk::WriteDescriptorSet, 1> primitivesDescriptorWrites {
				vk::WriteDescriptorSet {m_lineDescriptorSets.at(i), 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo, {}}
			};


			m_logicalDevice.getDevice().updateDescriptorSets(graphicDescriptorWrites, {});
			m_logicalDevice.getDevice().updateDescriptorSets(primitivesDescriptorWrites, {});
		}
	}

	void Renderer::createTextureImage()
	{
		int texWidth = 0;
		int texHeight = 0;
		int texChannels = 0;

		stbi_uc* pixels = stbi_load("../Assets/Textures/texture2.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		vk::DeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
		{
			throw std::runtime_error("Failed to load texture image!");
		}

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;

		m_memoryManager.createBuffer(imageSize,
									 vk::BufferUsageFlagBits::eTransferSrc,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 stagingBuffer,
									 stagingBufferMemory);

		void* data = m_logicalDevice.getDevice().mapMemory(stagingBufferMemory, 0, imageSize);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		m_logicalDevice.getDevice().unmapMemory(stagingBufferMemory);

		stbi_image_free(pixels);

		m_imageManager.createImage(texWidth,
								   texHeight,
								   vk::Format::eR8G8B8A8Srgb,
								   vk::ImageTiling::eOptimal,
								   vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
								   vk::MemoryPropertyFlagBits::eDeviceLocal,
								   m_textureImage,
								   m_textureImageMemory);

		m_imageManager.transitionImageLayout(m_textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

		m_imageManager.copyBufferToImage(stagingBuffer, m_textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		m_imageManager.transitionImageLayout(m_textureImage,
											 vk::Format::eR8G8B8A8Srgb,
											 vk::ImageLayout::eTransferDstOptimal,
											 vk::ImageLayout::eShaderReadOnlyOptimal);

		m_logicalDevice.getDevice().destroyBuffer(stagingBuffer);
		m_logicalDevice.getDevice().freeMemory(stagingBufferMemory);
	}

	void Renderer::createTextureImageView()
	{
		m_textureImageView = m_imageManager.createImageView(m_textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
	}

	void Renderer::createTextureSampler()
	{
		vk::PhysicalDeviceProperties properties = m_physicalDevice.getPhysicalDevice().getProperties();

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

		m_textureSampler = m_logicalDevice.getDevice().createSampler(sampleInfo);
	}

	void Renderer::loadModel()
	{

		//TODO Change from load vertices and indices
		//to models and objects
		io::ImporterProxy importerProxy;
		importerProxy.readFile("../Assets/Models/Cube.obj");
		m_vertices = importerProxy.getVertices();
		m_indices = importerProxy.getIndices();
	}

	void Renderer::updateGeometry()
	{

		static float time { 0.0F };


		constexpr float amplitude { 0.01F };
		constexpr float frequency { 0.1F };
		const float moveX = amplitude * std::cos(frequency * time);
		//std::cout << moveX << std::endl;

		for (auto& vertex : m_vertices)
		{
			vertex.m_pos.x += moveX;
		}


		time += 0.1F;
		createVertexBuffer();
	}

	void Renderer::recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
	{
		commandBuffer.begin(vk::CommandBufferBeginInfo {});

		vk::ClearColorValue colorClean {
			std::array<float, 4> {0.0F, 0.0F, 0.0F, 1.0F}
		};
		vk::ClearDepthStencilValue depthClean { 1.0F, 0 };

		std::array<vk::ClearValue, 2> clearValues;
		clearValues[0].setColor(colorClean);
		clearValues[1].setDepthStencil(depthClean);


		const auto swapchainFramebuffers = m_framebuffer.getSwapchainFramebuffersBuffers();
		//Draw primitive
		vk::Extent2D swapChainExtent = m_swapChain.getSwapchainExtend2D();
		vk::RenderPassBeginInfo renderPassInfo { m_renderPass.getRenderPass(),
												 swapchainFramebuffers[imageIndex],
												 vk::Rect2D((0, 0), swapChainExtent),
												 clearValues };

		commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

		vk::Viewport viewport { 0.0F, 0.0F, static_cast<float>(swapChainExtent.width), static_cast<float>(swapChainExtent.height), 0.0F, 1.0F };

		vk::Rect2D scissor {
			{0, 0},
			swapChainExtent
		};
		commandBuffer.setViewport(0, 1, &viewport);
		commandBuffer.setScissor(0, 1, &scissor);


		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.getGraphicsPipeline());

		vk::Buffer vertexBuffers[] = { m_vertexBuffer };
		vk::DeviceSize offsets[] = { 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
		commandBuffer.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint32);

		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.getPipelineLayout(), 0, m_descriptorSets[currentFrame], {});

		commandBuffer.drawIndexed(static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);


		//-------------------Line----------------------------------

		//Bind Line pipeline
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_primitivesGraphicPipeline.getGraphicsPipeline());
		//
		//Bind Vertex Buffer from line
		std::array<vk::Buffer, 1> lineVertexBuffers { m_lineVertexBuffer };
		std::array<vk::DeviceSize, 1> lineVertexBuffersoffsets { 0 };

		commandBuffer.bindVertexBuffers(0, lineVertexBuffers, lineVertexBuffersoffsets);
		commandBuffer.bindIndexBuffer(m_lineIndexBuffer, 0, vk::IndexType::eUint32);
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
										 m_primitivesGraphicPipeline.getPipelineLayout(),
										 0,
										 m_lineDescriptorSets[currentFrame],
										 {});
		commandBuffer.drawIndexed(static_cast<uint32_t>(m_linesIndices.size()), 1, 0, 0, 0);


		commandBuffer.endRenderPass();
		commandBuffer.end();
	}


	void Renderer::mousePressEvent(int64_t x, int64_t y, Camera::Actions action)
	{
		m_camera.mousePressEvent(x, y, action);
	}

	void Renderer::mouseMoveEvent(int64_t x, int64_t y)
	{
		m_camera.mouseMove(x, y);
	}

	void Renderer::mouseReleaseEvent(int64_t x, int64_t y)
	{
		m_camera.mouseMove(x, y);
		m_camera.releaseMouseClick();
	}


}