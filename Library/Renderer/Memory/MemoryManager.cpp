#include "MemoryManager.hpp"

namespace st::renderer
{
	MemoryManager::MemoryManager(const vk::PhysicalDevice& physicalDevice,
								 const vk::Device& device,
								 const vk::CommandPool& commandPool,
								 const vk::Queue& queue):
		m_physicalDevice(physicalDevice),
		m_device(device),
		m_commandPool(commandPool),
		m_queue(queue)
	{ }


	uint32_t MemoryManager::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
	{
		vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}


	void MemoryManager::createBuffer(vk::DeviceSize size,
									 vk::BufferUsageFlags usage,
									 vk::MemoryPropertyFlags properties,
									 vk::Buffer& buffer,
									 vk::DeviceMemory& bufferMemory) const
	{

		vk::BufferCreateInfo bufferInfo { {}, size, usage, vk::SharingMode::eExclusive };

		buffer = m_device.createBuffer(bufferInfo);

		vk::MemoryRequirements memoryRequirements = m_device.getBufferMemoryRequirements(buffer);

		vk::MemoryAllocateInfo allocInfo { memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, properties) };

		bufferMemory = m_device.allocateMemory(allocInfo);


		m_device.bindBufferMemory(buffer, bufferMemory, 0);
	}

	void MemoryManager::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
	{
		vk::CommandBufferAllocateInfo allocInfo { m_commandPool, vk::CommandBufferLevel::ePrimary, 1 };

		auto commandBuffer = m_device.allocateCommandBuffers(allocInfo);

		vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

		commandBuffer.at(0).begin(beginInfo);

		vk::BufferCopy copyRegin { 0, 0, size };

		commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

		commandBuffer.at(0).end();

		vk::SubmitInfo submitInfo { {}, {}, commandBuffer };

		m_queue.submit(1, &submitInfo, {}); //TODO GraphicQueue should copy buffers?
		m_queue.waitIdle();

		m_device.freeCommandBuffers(m_commandPool, commandBuffer);
	}


}