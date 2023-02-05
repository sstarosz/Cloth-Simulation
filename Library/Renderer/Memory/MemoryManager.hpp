#ifndef RENDERER_MEMORY_MEMORYMANAGER_HPP
#define RENDERER_MEMORY_MEMORYMANAGER_HPP

#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::renderer {

class MemoryManager
{

	public:
		MemoryManager(const vk::PhysicalDevice& physicalDevice);

		void initialize();
		void releaseResources();


		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;

	private:
		const vk::PhysicalDevice& m_physicalDevice;
};


};

#endif // RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
