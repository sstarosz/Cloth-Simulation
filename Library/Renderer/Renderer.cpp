#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

Renderer::Renderer(const StInstance& instance, const Surface& surface):
m_instance(instance),
m_surface(surface),
m_physicalDevice(m_instance.getInstance(), surface.getSurface())
{

}


void Renderer::initialize()
{
	m_physicalDevice.initialize();

}

void Renderer::releaseResources() 
{
	m_physicalDevice.releaseResources();
}


vk::Instance Renderer::getInstance() const
{
	return m_instance.getInstance();
}

vk::SurfaceKHR Renderer::getSurface() const
{
	 return m_surface.getSurface();
}

vk::PhysicalDevice Renderer::getPhysicalDevice() const
{
	 return m_physicalDevice.getPhysicalDevice();
}

}