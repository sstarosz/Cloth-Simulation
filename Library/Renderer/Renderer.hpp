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
#include "CommandBuffers/CommandPool.hpp"
#include "IO/ImporterProxy.hpp"
#include "VulkanImages/ImageMenager.hpp"
#include "Memory/MemoryManager.hpp"
#include <Geometry/Vertex.hpp>
#include <Geometry/Matrix4x4.hpp>
#include "Framebuffer.hpp"
#include "Camera.hpp"
#include <optional>

namespace st::renderer
{

class Renderer
{
public:	

	Renderer(const StInstance& instance, const Surface& surface);

	void initialize();
	void releaseResources();



	//beginFrame -> offset rendering
	//endFrame

	void renderFrame();

	vk::Instance getInstance() const;


	void updateSwapChain(uint64_t width, uint64_t height);
	void recreateSwapChain(uint64_t width, uint64_t height);



	//TODO - What to do it with this?
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createCommandBuffers();
	void createSyncObjects();
	void createDescriptorPool();
	void createDescriptorSets();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();

	void loadModel();


	void updateUniformBuffer(uint32_t currentImage);
	void updateGeometry();
	void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);



	//TODO - Delete after refactor
	const vk::SurfaceKHR& getSurface() const;
	const vk::PhysicalDevice& getPhysicalDevice() const;
	const vk::Device& getLogicalDevice() const;
	const vk::Queue& getGraphicsQueue() const;
	const vk::Queue& getPresentationQueue() const;


	const vk::SwapchainKHR& getSwapchain() const;
	const vk::Extent2D& getSwapchainExtend2D() const;
	const vk::Format& getSwapChainImageFormat() const;
	const std::vector<vk::Image>& getSwapChainImages() const;
	const std::vector<vk::ImageView>& getSwapChainImagesViews() const;

	const vk::RenderPass& getRenderPass() const;
	const vk::Pipeline& getGraphicsPipeline() const;
	const vk::PipelineLayout& getPipelineLayout() const;
	const vk::DescriptorSetLayout& getDescriptorSetLayout() const;
	const vk::CommandPool& getCommandPool() const;

	const std::vector<vk::Framebuffer> getSwapchainFramebuffersBuffers() const;
		
private:
	const StInstance& m_instance;
	const Surface& m_surface;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
	SwapChain m_swapChain;
	RenderPass m_renderPass;
	GraphicsPipeline m_graphicPipeline;
	CommandPool m_commandPool;

	MemoryManager m_memoryManager;
	ImageMenager m_imageManager;
	Framebuffer m_framebuffer;


	uint32_t currentFrame = 0;
	bool m_framebufferResized = false;
	const static uint32_t MAX_FRAMES_IN_FLIGHT = 2;

	//Synchronization
	std::vector<vk::Semaphore> m_imageAvailableSemaphores;
	std::vector<vk::Semaphore> m_renderFinishedSemaphores;
	std::vector<vk::Fence> m_inFlightFences;


	//Geometry
	//TO-DO class to hold geometry?
	std::vector<geometry::Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	vk::Buffer m_vertexBuffer;
	vk::DeviceMemory m_vertexBufferMemory;
	vk::Buffer m_indexBuffer;
	vk::DeviceMemory m_indexBufferMemory;


	//Uniform buffers
	struct UniformBufferObject
	{
			geometry::Matrix4x4 model;
			geometry::Matrix4x4 view;
			geometry::Matrix4x4 proj;
	};

	std::vector<vk::Buffer> m_uniformBuffers;
	std::vector<vk::DeviceMemory> m_uniformBuffersMemory;


	//Camera in Renderer or viewport?
	Camera m_camera;


	//Command buffers
	std::vector<vk::CommandBuffer> m_commandBuffers;

	//Descriptors Sets
	vk::DescriptorPool m_descriptorPool;
	std::vector<vk::DescriptorSet> m_descriptorSets;


	//Images
	vk::Image m_textureImage;
	vk::DeviceMemory m_textureImageMemory;
	vk::ImageView m_textureImageView;
	vk::Sampler m_textureSampler;
};

}


#endif // !RENDERER_RENDERER_HPP