#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

Renderer::Renderer(const StInstance& instance, const Surface& surface):
m_instance(instance),
m_surface(surface),
m_physicalDevice(m_instance.getInstance(), surface.getSurface()),
m_logicalDevice(m_instance.getInstance(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
m_swapChain(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(),m_logicalDevice.getDevice()),
m_renderPass(m_physicalDevice.getPhysicalDevice(), m_swapChain.getSwapChainImageFormat(), m_logicalDevice.getDevice()),
m_graphicPipeline(m_physicalDevice.getPhysicalDevice(), m_logicalDevice.getDevice(), m_renderPass.getRenderPass()),
m_commandPool(m_logicalDevice.getDevice(), m_physicalDevice.getPhysicalDevice(),m_surface.getSurface()),
m_memoryManager(m_physicalDevice.getPhysicalDevice()),
m_imageManager(m_logicalDevice.getDevice(), m_memoryManager),
m_framebuffer(m_logicalDevice.getDevice(), m_swapChain, m_renderPass, m_imageManager)
{
}


void Renderer::initialize()
{
	m_physicalDevice.initialize();
	m_logicalDevice.initialize();
	m_swapChain.initialize();
	m_renderPass.initialize();
	m_graphicPipeline.initialize();
	m_commandPool.initialize();
	m_framebuffer.initialize();
}

void Renderer::releaseResources() 
{
	//Reverse order then intialization
	m_framebuffer.releaseResources();
	m_commandPool.releaseResources();
	m_graphicPipeline.releaseResources();
	m_renderPass.releaseResources();
	m_swapChain.releaseResources();
	m_logicalDevice.releaseResources();
	m_physicalDevice.releaseResources();
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
	m_framebuffer.releaseResources();
	m_swapChain.recreateSwapChain(width, height);
	m_framebuffer.initialize();
}

const vk::SurfaceKHR& Renderer::getSurface() const
{
	 return m_surface.getSurface();
}

const vk::PhysicalDevice& Renderer::getPhysicalDevice() const
{
	 return m_physicalDevice.getPhysicalDevice();
}

const vk::Device& Renderer::getLogicalDevice() const
{
	 return m_logicalDevice.getDevice();
}

const vk::Queue& Renderer::getGraphicsQueue() const
{
	 return m_logicalDevice.getGraphiceQueue();
}

const vk::Queue& Renderer::getPresentationQueue() const
{
	 return m_logicalDevice.getPresentQueue();
}

const vk::SwapchainKHR& Renderer::getSwapchain() const
{
	 return m_swapChain.getSwapchain();
}

const vk::Extent2D& Renderer::getSwapchainExtend2D() const
{
	 return  m_swapChain.getSwapchainExtend2D();
}

const vk::Format& Renderer::getSwapChainImageFormat() const
{
	 return m_swapChain.getSwapChainImageFormat();
}

const std::vector<vk::Image>& Renderer::getSwapChainImages() const
{
	 return m_swapChain.getSwapChainImages();
}

const std::vector<vk::ImageView>& Renderer::getSwapChainImagesViews() const
{
	 return m_swapChain.getSwapChainImagesViews();
}

const vk::RenderPass& Renderer::getRenderPass() const
{
	 return m_renderPass.getRenderPass();
}

const vk::Pipeline& Renderer::getGraphicsPipeline() const
{
	 return m_graphicPipeline.getGraphicsPipeline();
}

const vk::PipelineLayout& Renderer::getPipelineLayout() const
{
	 return m_graphicPipeline.getPipelineLayout();
}

const vk::DescriptorSetLayout& Renderer::getDescriptorSetLayout() const
{
	 return m_graphicPipeline.getDescriptorSetLayout();
}


const vk::CommandPool& Renderer::getCommandPool() const
{
	 return m_commandPool.getCommandPool();
}

const std::vector<vk::Framebuffer> Renderer::getSwapchainFramebuffersBuffers() const
{
	 return m_framebuffer.getSwapchainFramebuffersBuffers();
}

}