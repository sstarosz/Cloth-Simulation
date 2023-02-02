#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

Renderer::Renderer(const StInstance& instance, const Surface& surface):
m_instance(instance),
m_surface(surface),
m_physicalDevice(m_instance.getInstance(), surface.getSurface()),
m_logicalDevice(m_instance.getInstance(), m_physicalDevice.getPhysicalDevice(), m_surface.getSurface())
{

}


void Renderer::initialize()
{
	m_physicalDevice.initialize();
	m_logicalDevice.initialize();

}

void Renderer::releaseResources() 
{
	m_physicalDevice.releaseResources();
	m_logicalDevice.releaseResources();
}


vk::Instance Renderer::getInstance() const
{
	return m_instance.getInstance();
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

}