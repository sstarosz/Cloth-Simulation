#include "DebugMessenger.hpp"

#include <iostream>


//TODO - check if this need to be in global namespace
PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;


[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger
)
{
	return pfnVkCreateDebugUtilsMessengerEXT(
		instance, pCreateInfo, pAllocator, pMessenger
	);
}

[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT messenger,
	VkAllocationCallbacks const* pAllocator
)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}


namespace st::renderer
{

DebugMessenger::DebugMessenger(const vk::Instance& instance):
	m_instance(instance),
	m_debugMessenger()
{
}


VkBool32 DebugMessenger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
									   VkDebugUtilsMessageTypeFlagsEXT messageType,
									   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
									   void* pUserData
)
{
	std::ostringstream message;

	message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) 
			<< ": "
			<< vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType))
			<< ":\n";
	message << "\t"
			<< "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
	message << "\t"
			<< "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
	message << "\t"
			<< "message         = <" << pCallbackData->pMessage << ">\n";

	if (0 < pCallbackData->queueLabelCount)
	{
		message << "\t"
				<< "Queue Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
		{
			message << "\t\t"
					<< "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName
					<< ">\n";
		}
	}

	if (0 < pCallbackData->cmdBufLabelCount)
	{
		message << "\t"
				<< "CommandBuffer Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
		{
			message << "\t\t"
					<< "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName
					<< ">\n";
		}
	}

	if (0 < pCallbackData->objectCount)
	{
		message << "\t"
				<< "Objects:\n";
		for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
		{
			message << "\t\t"
					<< "Object " << i << "\n";
			message << "\t\t\t"
					<< "objectType   = "
					<< vk::to_string(static_cast<vk::ObjectType>(
						   pCallbackData->pObjects[i].objectType
					   ))
					<< "\n";
			message << "\t\t\t"
					<< "objectHandle = " << pCallbackData->pObjects[i].objectHandle
					<< "\n";
			if (pCallbackData->pObjects[i].pObjectName)
			{
				message << "\t\t\t"
						<< "objectName   = <" << pCallbackData->pObjects[i].pObjectName
						<< ">\n";
			}
		}
	}

	std::cout << message.str() << std::endl;

	return false;
}

void DebugMessenger::initialize()
{
	 pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( m_instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
	 if (!pfnVkCreateDebugUtilsMessengerEXT)
	 {
		//TODO - Change it to something independed of iostream
		std::cout << "GetInstanceProcAddr: Unable to find "
					 "pfnVkCreateDebugUtilsMessengerEXT function."
				  << std::endl;
		exit(1);
	 }

	 pfnVkDestroyDebugUtilsMessengerEXT= reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>( m_instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
	 if (!pfnVkDestroyDebugUtilsMessengerEXT)
	 {
		//TODO - Change it to something independed of iostream
		std::cout << "GetInstanceProcAddr: Unable to find "
					 "pfnVkDestroyDebugUtilsMessengerEXT function."
				  << std::endl;
		exit(1);
	 }


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
		 {}, severityFlags, messageTypeFlags, &debugCallback
	 };


	 m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);

}

void DebugMessenger::releaseResources() 
{
	m_instance.destroy(m_debugMessenger);
}




}