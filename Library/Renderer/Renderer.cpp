#include "Renderer.hpp"
#include "vulkan/vulkan.hpp"

#include "ValidationLayers.hpp"
#include "Extensions.hpp"

namespace st::renderer {



void Renderer::initialize()
{

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
		ValidationLayer::getValidationLayers(), enabledExtensions
	};

}

}