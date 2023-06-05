#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"
#include "RenderPass/RenderPass.hpp"
#include "Pipeline/GraphicsPipeline.hpp"
#include "Pipeline/PrimitivesGraphicsPipeline.hpp"
#include "CommandBuffers/CommandPool.hpp"
#include "IO/ImporterProxy.hpp"
#include "VulkanImages/ImageManager.hpp"
#include "Memory/MemoryManager.hpp"
#include <Geometry/Vertex.hpp>
#include <Geometry/Matrix4x4.hpp>
#include <Geometry/Line.hpp>
#include "Framebuffer.hpp"
#include "Camera.hpp"
#include <optional>
#include "Core/ModelsManager.hpp"

namespace st::renderer
{

	class Renderer
	{
	public:
		Renderer(const StInstance& instance, const Surface& surface, const core::ModelsManager& modelMenager);

		void initialize();
		void releaseResources();


		void updateRecourses();


		//beginFrame -> offset rendering
		//endFrame

		void renderFrame();

		vk::Instance getInstance() const;


		void updateSwapChain(uint64_t width, uint64_t height);
		void recreateSwapChain(uint64_t width, uint64_t height);


		//Add primitives to renderer
		//gird
		//gizmo
		//


		//TODO - What to do it with this?
		void createVertexBuffer();
		void createIndexBuffer();
		void createCommandBuffers();
		void createSyncObjects();


		void updateUniformBuffer(uint32_t currentImage);
		void updateGeometry();
		void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);


		void mousePressEvent(int64_t x, int64_t y, Camera::Actions action);
		void mouseMoveEvent(int64_t x, int64_t y);
		void mouseReleaseEvent(int64_t x, int64_t y);


	private:
		const StInstance& m_instance;
		const Surface& m_surface;
		const core::ModelsManager& m_modelManager;

		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;
		SwapChain m_swapChain;
		RenderPass m_renderPass;
		MemoryManager m_memoryManager;
		ImageManager m_imageManager;
		GraphicsPipeline m_graphicPipeline;
		PrimitivesGraphicsPipeline m_primitivesGraphicPipeline;
		CommandPool m_commandPool;


		Framebuffer m_framebuffer;


		uint32_t currentFrame = 0;
		bool m_framebufferResized = false;
		const static uint32_t MAX_FRAMES_IN_FLIGHT = 2; //TODO this should be define by swapchain

		//Synchronization
		std::vector<vk::Semaphore> m_imageAvailableSemaphores;
		std::vector<vk::Semaphore> m_renderFinishedSemaphores;
		std::vector<vk::Fence> m_inFlightFences;


		//Camera in Renderer or viewport?
		Camera m_camera;


		//Command buffers
		std::vector<vk::CommandBuffer> m_commandBuffers;

		//Line
		vk::Buffer m_lineVertexBuffer;
		vk::DeviceMemory m_lineVertexBufferMemory;
		vk::Buffer m_lineIndexBuffer;
		vk::DeviceMemory m_lineIndexBufferMemory;

		std::vector<vk::DescriptorSet> m_lineDescriptorSets;

		const std::vector<geometry::Line> m_lines = {
			{{ -2.0F, 0.0F, 0.0F },  { 1.0F, 0.0F, 0.0F }},
            { { 2.0F, 0.0F, 0.0F },  { 1.0F, 0.0F, 0.0F }}, //x - axis
			{ { 0.0F, -2.0F, 0.0F }, { 0.0F, 1.0F, 0.0F }},
            { { 0.0F, 2.0F, 0.0F },  { 0.0F, 1.0F, 0.0F }}, //y - axis
			{ { 0.0F, 0.0F, -2.0F }, { 0.0F, 0.0F, 1.0F }},
            { { 0.0F, 0.0F, 2.0F },  { 0.0F, 0.0F, 1.0F }}  //z - axis
		};

		std::vector<uint32_t> m_linesIndices = { 0, 1, 2, 3, 4, 5 };


		//void addModel(const viewport::Model& mesh);
		//void createTextureImage(viewport::Texture texture, vk::Image& textureImage, vk::DeviceMemory& textureImageMemory);
		//void createTextureImageView(vk::Image& textureImage, vk::ImageView& textureImageView);


		struct RenderableMesh
		{
				vk::Buffer vertexBuffer;
				vk::DeviceMemory vertexBufferMemory;

				uint32_t indicesSize;
				vk::Buffer indexBuffer;
				vk::DeviceMemory indexBufferMemory;

				vk::Image textureImage;
				vk::DeviceMemory textureImageMemory;
				vk::ImageView textureImageView;

				std::span<std::byte> mappedVertexMemory;
				std::span<std::byte> mappedIndexMemory;

				std::vector<vk::DescriptorSet> descriptorSets;
		};		

		std::vector<RenderableMesh> m_renderableMeshes;

	};

}


#endif // !RENDERER_RENDERER_HPP