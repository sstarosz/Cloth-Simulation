#include "MemoryManager.hpp"

namespace st::renderer 
{
	MemoryManager::MemoryManager(const vk::PhysicalDevice& physicalDevice):
	m_physicalDevice(physicalDevice)
	{
	}


	uint32_t MemoryManager::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
	{
		vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}



}