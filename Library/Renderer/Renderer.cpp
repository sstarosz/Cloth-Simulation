#include "Renderer.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {

	Renderer::Renderer():
	m_instance(),
	m_debugMessenger(m_instance)
	{

	}

void Renderer::initialize()
{
	createInstance();
	m_debugMessenger.initialize();
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
}

vk::Instance Renderer::getInstance()
{
	 return m_instance;
}

}