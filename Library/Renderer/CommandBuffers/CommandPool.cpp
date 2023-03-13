#include "CommandPool.hpp"
#include "Renderer/QueueFamily.hpp"


namespace st::renderer
{

	CommandPool::CommandPool(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface):
		m_device(device),
		m_physicalDevice(physicalDevice),
		m_surface(surface)
	{ }

	void CommandPool::initialize()
	{
		QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);

		//TODO This is a graphic commandPoll
		vk::CommandPoolCreateInfo poolInfo { vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndices.graphicsFamily.value() };

		m_commandPool = m_device.createCommandPool(poolInfo);
	}

	void CommandPool::releaseResources()
	{
		m_device.destroyCommandPool(m_commandPool);
	}

	const vk::CommandPool& CommandPool::getCommandPool() const
	{
		return m_commandPool;
	}


}