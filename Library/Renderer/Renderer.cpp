#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

	Renderer::Renderer():
	m_instance(),
	m_debugMessenger(m_instance),
	m_surface(),
	m_physicalDevice()
	{

	}

void Renderer::setupSurface(const vk::SurfaceKHR& surface)
{ 
	m_surface.emplace(surface);
}

void Renderer::initialize()
{
	m_physicalDevice = PhysicalDevice(m_instance, m_surface->getSurface());
	m_physicalDevice.initialize();

}

void Renderer::releaseResources() 
{
	
	m_debugMessenger.releaseResources();
	m_instance.destroy();
}



void Renderer::createInstance()
{
	vk::ApplicationInfo appInfo {
		"Cloth Simulation App", 
		VK_MAKE_API_VERSION(1, 0, 0, 0),
		"No Engine",
		VK_MAKE_API_VERSION(1U, 0U, 0U, 0U),
		VK_API_VERSION_1_3
	};


	auto validationLayers = ValidationLayer::getValidationLayers();
	auto enabledExtensions = Extensions::getEnabledExtensions();


	 vk::InstanceCreateInfo instanceCreateInfo {
		{},
		&appInfo,
		validationLayers,
		enabledExtensions
	};


	  m_instance = vk::createInstance(instanceCreateInfo);

	  m_debugMessenger.initialize();
}



vk::Instance Renderer::getInstance() const
{
	 return m_instance;
}

vk::SurfaceKHR Renderer::getSurface() const
{
	 return m_surface->getSurface();
}

vk::PhysicalDevice Renderer::getPhysicalDevice() const
{
	 return m_physicalDevice.getPhysicalDevice();
}

}