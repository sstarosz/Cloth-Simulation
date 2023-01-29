#ifndef RENDERER_DEBUGMESSENGER_HPP
#define RENDERER_DEBUGMESSENGER_HPP

#include <vulkan/vulkan.hpp>


namespace st::renderer {

/*TODO add validationLayer enable flags*/

class DebugMessenger
{

public:
	 DebugMessenger(const vk::Instance& m_instance);
	
	//VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
	//															   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	//															   const VkAllocationCallbacks* pAllocator,
	//															   VkDebugUtilsMessengerEXT* pMessenger);
	//
	//VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
	//															VkDebugUtilsMessengerEXT messenger,
	//															VkAllocationCallbacks const* pAllocator);
	//
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
														VkDebugUtilsMessageTypeFlagsEXT messageType,
														const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
														void* pUserData);
	
	void initialize();
	void releaseResources();
 private:
	const vk::Instance& m_instance;


	vk::DebugUtilsMessengerEXT m_debugMessenger;
};


}


#endif // !RENDERER_DEBUGMESSENGER_HPP