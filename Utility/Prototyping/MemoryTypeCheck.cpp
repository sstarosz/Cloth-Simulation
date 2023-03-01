#include <iostream>
#include <vulkan/vulkan.hpp>


//Instance

std::vector<const char*>getValidationLayers()
{
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	return validationLayers;
}


std::vector<const char*> getRequiredExtensions()
{
	std::vector<const char*> extensions;

	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	
	return extensions;
}


vk::Instance createInstance()
{
	vk::ApplicationInfo applicationInfo {
		"MemoryTypeCheck",
		VK_MAKE_API_VERSION(1, 0, 0, 0), 
		"Vulkan",
		VK_MAKE_API_VERSION(1U, 0U, 0U, 0U),
		VK_API_VERSION_1_3 
	};


	const auto extensions = getRequiredExtensions();
	const auto validationLayers = getValidationLayers();


	  vk::InstanceCreateInfo instanceCreateInfo(
		{},
		&applicationInfo,
		validationLayers,
		extensions
	);

	return { vk::createInstance(instanceCreateInfo) };
}


//Debug
PFN_vkCreateDebugUtilsMessengerEXT m_pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT m_pfnVkDestroyDebugUtilsMessengerEXT;

[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger
)
{
	return m_pfnVkCreateDebugUtilsMessengerEXT(
		instance, pCreateInfo, pAllocator, pMessenger
	);
}

[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT messenger,
	VkAllocationCallbacks const* pAllocator
)
{
	return m_pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VkBool32 debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << "\n\n";

	return VK_FALSE;
}


vk::DebugUtilsMessengerEXT setupDebugMessenger(const vk::Instance& instance)
{
	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags {
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
		| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
		| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
	};

	vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags {
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
		| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
		| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
	};

	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT {
		{},
		severityFlags,
		messageTypeFlags,
		&debugCallback
	};

	m_pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
	if (!m_pfnVkCreateDebugUtilsMessengerEXT)
	{
		std::cout << "GetInstanceProcAddr: Unable to find "
					 "pfnVkCreateDebugUtilsMessengerEXT function."
				  << std::endl;
		exit(1);
	}

	m_pfnVkDestroyDebugUtilsMessengerEXT
		= reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")
		);
	if (!m_pfnVkDestroyDebugUtilsMessengerEXT)
	{
		std::cout << "GetInstanceProcAddr: Unable to find "
					 "pfnVkDestroyDebugUtilsMessengerEXT function."
				  << std::endl;
		exit(1);
	}

	return instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);

}



int main() {




	vk::Instance instance = createInstance();
	vk::DebugUtilsMessengerEXT m_debugMessenger = setupDebugMessenger(instance);



	 std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
	for (const auto& device: devices)
	{

		const auto deviceLayouProperties = device.enumerateDeviceLayerProperties();
		for (const auto& deviceLayouPropertie : deviceLayouProperties)
		{
			std::cout << "Desciption: " << deviceLayouPropertie.description << std::endl;
			std::cout << "Implementation Version: " << deviceLayouPropertie.implementationVersion << std::endl;
			std::cout << "Layer Name: "<< deviceLayouPropertie.layerName << std::endl;
			std::cout << std::endl;
		}

		//device.
		const auto features = device.getFeatures();
		const auto features2 = device.getFeatures2();



		const auto memoryProperties = device.getMemoryProperties2();
		const auto memoryHeaps = memoryProperties.memoryProperties.memoryHeaps;
		const auto memoryHeapsCount = memoryProperties.memoryProperties.memoryHeapCount;

		const auto memoryTypes = memoryProperties.memoryProperties.memoryTypes;
		const auto memoryTypesCount = memoryProperties.memoryProperties.memoryTypeCount;



		for (size_t i = 0; i < memoryHeapsCount; i++)
		{
			std::cout << "Memory Heap Flags: " << vk::to_string(memoryHeaps.at(i).flags) << std::endl;
			std::cout << "Memory Size: " << memoryHeaps.at(i).size << std::endl;
			std::cout << std::endl;
		}

		for (size_t i = 0; i < memoryTypesCount; i++)
		{
			std::cout << "Property Flags: " << vk::to_string(memoryTypes.at(i).propertyFlags) << std::endl;
			std::cout << "HeapIndex: " << memoryTypes.at(i).heapIndex << std::endl;
			std::cout << std::endl;
		}
		//features.

		//device.
	}

	std::cout << "Hello, world" << std::endl;



    instance.destroyDebugUtilsMessengerEXT(m_debugMessenger);
	instance.destroy();
	return 0;
}