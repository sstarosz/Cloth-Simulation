#include "Renderer.hpp"
#include "ValidationLayers.hpp"
#include "Extensions.hpp"
#include "UniformBuffers.hpp"
#include <iostream>
#include <chrono>

namespace st::renderer
{

	Renderer::Renderer(const StInstance& instance, const Surface& surface, const viewport::ModelsMenager& modelMenager):
		m_instance(instance),
		m_surface(surface),
		m_modelMenager(modelMenager),
		m_physicalDevice(m_instance.getInstance(), surface.getSurface()),
		m_logicalDevice(m_instance.getInstance(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
		m_swapChain(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(), m_logicalDevice.getDevice()),
		m_renderPass(m_physicalDevice.getPhysicalDevice(), m_swapChain.getSwapChainImageFormat(), m_logicalDevice.getDevice()),
		m_memoryManager(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_commandPool.getCommandPool(), m_logicalDevice.getGraphiceQueue()),
		m_imageManager(m_logicalDevice.getDevice(), m_commandPool.getCommandPool(), m_logicalDevice.getGraphiceQueue(), m_memoryManager),
		m_graphicPipeline(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_renderPass.getRenderPass(),m_memoryManager),
		m_primitivesGraphicPipeline(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_renderPass.getRenderPass(), m_memoryManager),
		m_commandPool(m_logicalDevice.getDevice(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
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


		//Initialize primitives (gizmo, grid, axis, etc)
		createVertexBuffer();
		createIndexBuffer();
		createCommandBuffers();
		createSyncObjects();
	}

	void Renderer::releaseResources()
	{
		m_logicalDevice.getDevice().waitIdle();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_logicalDevice.getDevice().destroySemaphore(m_renderFinishedSemaphores[i]);
			m_logicalDevice.getDevice().destroySemaphore(m_imageAvailableSemaphores[i]);
			m_logicalDevice.getDevice().destroyFence(m_inFlightFences[i]);
		}

		m_logicalDevice.getDevice().destroyBuffer(m_lineIndexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_lineIndexBufferMemory);


		m_logicalDevice.getDevice().destroyBuffer(m_lineVertexBuffer);
		m_logicalDevice.getDevice().freeMemory(m_lineVertexBufferMemory);

		for (auto& renderableMesh : m_renderableMeshes)
		{
			m_logicalDevice.getDevice().unmapMemory(renderableMesh.vertexBufferMemory);
			m_logicalDevice.getDevice().unmapMemory(renderableMesh.indexBufferMemory);

			m_logicalDevice.getDevice().freeMemory(renderableMesh.vertexBufferMemory);
			m_logicalDevice.getDevice().freeMemory(renderableMesh.indexBufferMemory);


			m_logicalDevice.getDevice().destroyBuffer(renderableMesh.vertexBuffer);
			m_logicalDevice.getDevice().destroyBuffer(renderableMesh.indexBuffer);

			m_logicalDevice.getDevice().destroyImageView(renderableMesh.textureImageView);

			m_logicalDevice.getDevice().destroyImage(renderableMesh.textureImage);
			m_logicalDevice.getDevice().freeMemory(renderableMesh.textureImageMemory);
		}


		//Reverse order then initialization
		m_framebuffer.releaseResources();
		m_commandPool.releaseResources();
		m_graphicPipeline.releaseResources();
		m_primitivesGraphicPipeline.releaseResources();
		m_renderPass.releaseResources();
		m_swapChain.releaseResources();
		m_logicalDevice.releaseResources();
		m_physicalDevice.releaseResources();
	}

	void Renderer::updateRecourses()
	{
		for (const auto& model : m_modelMenager.getModelsToRender())
		{
			addModel(model);
		}
	}

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

	void Renderer::createVertexBuffer()
	{

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
	}

	void Renderer::createIndexBuffer()
	{

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

	void Renderer::createCommandBuffers()
	{
		vk::CommandBufferAllocateInfo allocInfo { m_commandPool.getCommandPool(),
												  vk::CommandBufferLevel::ePrimary,
												  static_cast<uint32_t>(m_framebuffer.getSwapchainFramebuffersBuffers().size()) };

		m_commandBuffers = m_logicalDevice.getDevice().allocateCommandBuffers(allocInfo);
	}

	void Renderer::updateUniformBuffer(uint32_t currentImage)
	{
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


		void* data = m_logicalDevice.getDevice().mapMemory(m_graphicPipeline.getUniformBufferMemory(currentImage), 0, sizeof(ubo));
		memcpy(data, &ubo, sizeof(ubo));
		m_logicalDevice.getDevice().unmapMemory(m_graphicPipeline.getUniformBufferMemory(currentImage));


		data = m_logicalDevice.getDevice().mapMemory(m_primitivesGraphicPipeline.getUniformBufferMemory(currentImage), 0, sizeof(ubo));
		memcpy(data, &ubo, sizeof(ubo));
		m_logicalDevice.getDevice().unmapMemory(m_primitivesGraphicPipeline.getUniformBufferMemory(currentImage));
	}

	void Renderer::updateGeometry()
	{
		auto& dynamic_mesh = m_renderableMeshes.at(1);


		const auto updatedMesh = m_modelMenager.getModelsToRender().at(1).m_mesh;
		const vk::DeviceSize bufferSize = sizeof(updatedMesh.m_vertices[0]) * updatedMesh.m_vertices.size();


		std::memcpy(dynamic_mesh.mappedVertexMemory.data(),
					updatedMesh.m_vertices.data(),
					static_cast<size_t>(bufferSize)); //vertices should fulfill trivial object specification?
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

	void Renderer::recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
	{
		commandBuffer.begin(vk::CommandBufferBeginInfo {});

		const vk::ClearColorValue colorClean {
			std::array<float, 4> {0.0F, 0.0F, 0.0F, 1.0F}
		};
		const vk::ClearDepthStencilValue depthClean { 1.0F, 0 };

		std::array<vk::ClearValue, 2> clearValues { colorClean, depthClean };


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


		//-------------------Draw all objects----------------------------------
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicPipeline.getGraphicsPipeline());
		for (const auto& renderableMesh : m_renderableMeshes)
		{
			vk::Buffer vertexBuffers[] = { renderableMesh.vertexBuffer };
			vk::DeviceSize offsets[] = { 0 };

			commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
			commandBuffer.bindIndexBuffer(renderableMesh.indexBuffer, 0, vk::IndexType::eUint32);

			commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
											 m_graphicPipeline.getPipelineLayout(),
											 0,
											 renderableMesh.descriptorSets.at(currentFrame),
											 {});
			commandBuffer.drawIndexed(renderableMesh.indicesSize, 1, 0, 0, 0);
		}


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
										 m_primitivesGraphicPipeline.getDescriptorSet(currentFrame),
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

	void Renderer::addModel(const viewport::Model& mesh)
	{
		RenderableMesh renderableMesh; //Result


		/*Create Vertex Buffer for Mesh	*/
		const vk::DeviceSize bufferSize = sizeof(mesh.m_mesh.m_vertices[0]) * mesh.m_mesh.m_vertices.size();

		//TODO- check if i can add it to exisitng buffer.
		m_memoryManager.createBuffer(bufferSize,
									 vk::BufferUsageFlagBits::eVertexBuffer,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 renderableMesh.vertexBuffer,
									 renderableMesh.vertexBufferMemory);


		renderableMesh.mappedVertexMemory = { static_cast<std::byte*>(m_logicalDevice.getDevice().mapMemory(renderableMesh.vertexBufferMemory, 0, bufferSize)),
											  bufferSize };

		std::memcpy(renderableMesh.mappedVertexMemory.data(),
					mesh.m_mesh.m_vertices.data(),
					static_cast<size_t>(bufferSize)); //vertices should fulfill trivial object specification?


		//TODO
		//m_logicalDevice.getDevice().unmapMemory(renderableMesh.vertexBufferMemory);


		/*Create Index Buffer for Mesh*/
		renderableMesh.indicesSize = mesh.m_mesh.m_indices.size();
		vk::DeviceSize indexbufferSize = sizeof(mesh.m_mesh.m_indices[0]) * mesh.m_mesh.m_indices.size();


		m_memoryManager.createBuffer(indexbufferSize,
									 vk::BufferUsageFlagBits::eIndexBuffer,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 renderableMesh.indexBuffer,
									 renderableMesh.indexBufferMemory);


		renderableMesh.mappedIndexMemory = { static_cast<std::byte*>(
												 m_logicalDevice.getDevice().mapMemory(renderableMesh.indexBufferMemory, 0, indexbufferSize)),
											 indexbufferSize };
		std::memcpy(renderableMesh.mappedIndexMemory.data(), mesh.m_mesh.m_indices.data(), static_cast<size_t>(indexbufferSize));


		//If use texture
		//createTextureImage();
		/*Create Texture Image per Mesh		Create Texture Image View per Mesh*/
		createTextureImage(mesh.m_texture, renderableMesh.textureImage, renderableMesh.textureImageMemory);
		createTextureImageView(renderableMesh.textureImage, renderableMesh.textureImageView);


		/*Create Descriptor Pool*/
		// Should be one for all object?
		renderableMesh.descriptorSets = m_graphicPipeline.createDescriptorSetPerMesh();
		//m_graphicPipeline.getDescriptorSet();
		m_graphicPipeline.updateDescriptorSet(renderableMesh.descriptorSets, renderableMesh.textureImageView);
		

		m_renderableMeshes.emplace_back(renderableMesh);
	}

	void Renderer::createTextureImage(viewport::Texture texture, vk::Image& textureImage, vk::DeviceMemory& textureImageMemory)
	{

		vk::DeviceSize imageSize = texture.textureWidth * texture.textureHeight * 4;


		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;

		m_memoryManager.createBuffer(imageSize,
									 vk::BufferUsageFlagBits::eTransferSrc,
									 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
									 stagingBuffer,
									 stagingBufferMemory);

		void* data = m_logicalDevice.getDevice().mapMemory(stagingBufferMemory, 0, imageSize);
		memcpy(data, texture.pixels.data(), static_cast<size_t>(imageSize));
		m_logicalDevice.getDevice().unmapMemory(stagingBufferMemory);


		m_imageManager.createImage(texture.textureWidth,
								   texture.textureHeight,
								   vk::Format::eR8G8B8A8Srgb,
								   vk::ImageTiling::eOptimal,
								   vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
								   vk::MemoryPropertyFlagBits::eDeviceLocal,
								   textureImage,
								   textureImageMemory);

		m_imageManager.transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

		m_imageManager.copyBufferToImage(stagingBuffer, textureImage, texture.textureWidth, texture.textureHeight);

		m_imageManager.transitionImageLayout(textureImage,
											 vk::Format::eR8G8B8A8Srgb,
											 vk::ImageLayout::eTransferDstOptimal,
											 vk::ImageLayout::eShaderReadOnlyOptimal);

		m_logicalDevice.getDevice().destroyBuffer(stagingBuffer);
		m_logicalDevice.getDevice().freeMemory(stagingBufferMemory);
	}

	void Renderer::createTextureImageView(vk::Image& textureImage, vk::ImageView& textureImageView)
	{
		textureImageView = m_imageManager.createImageView(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
	}


}