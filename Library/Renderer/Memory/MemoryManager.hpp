#ifndef RENDERER_MEMORY_MEMORYMANAGER_HPP
#define RENDERER_MEMORY_MEMORYMANAGER_HPP

#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::renderer
{

	class MemoryManager
	{

	public:
		MemoryManager(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& m_commandPool, const vk::Queue& m_queue);

		void initialize();
		void releaseResources();


		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
		void
		createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) const;

		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

	private:
		const vk::PhysicalDevice& m_physicalDevice;
		const vk::Device& m_device;
		const vk::CommandPool& m_commandPool;
		const vk::Queue& m_queue;
	};


};

#endif // RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
