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
#include <optional>

namespace st::renderer
{

class Renderer
{
public:	

	Renderer(const StInstance& instance, const Surface& surface);

	void initialize();
	void releaseResources();


	vk::Instance getInstance() const;


	void updateSwapChain(uint64_t width, uint64_t height);
	void recreateSwapChain(uint64_t width, uint64_t height);




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

		
private:
	const StInstance& m_instance;
	const Surface& m_surface;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
	SwapChain m_swapChain;
	RenderPass m_renderPass;
	GraphicsPipeline m_graphicPipeline;
	CommandPool m_commandPool;
};

}


#endif // !RENDERER_RENDERER_HPP