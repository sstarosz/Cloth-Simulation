#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

Renderer::Renderer(const StInstance& instance, const Surface& surface):
m_instance(instance),
m_surface(surface),
m_physicalDevice(m_instance.getInstance(), surface.getSurface()),
m_logicalDevice(m_instance.getInstance(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface()),
m_swapChain(m_physicalDevice.getPhysicalDevice(), m_surface.getSurface(),m_logicalDevice.getDevice())
{

}


void Renderer::initialize()
{
	m_physicalDevice.initialize();
	m_logicalDevice.initialize();
	m_swapChain.initialize();

}

void Renderer::releaseResources() 
{
	m_physicalDevice.releaseResources();
	m_logicalDevice.releaseResources();
	m_swapChain.releaseResources();
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
	m_swapChain.recreateSwapChain(width, height);
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


}