#include "LogicalDevice.hpp"
#include "QueueFamily.hpp"
#include "ValidationLayers.hpp"
#include "Extensions.hpp"


#include <set>

namespace st::renderer {




LogicalDevice::LogicalDevice(
	const vk::Instance& instance,
	const vk::PhysicalDevice& physicalDevice,
	const vk::SurfaceKHR& surface
):
	m_instance(instance),
	m_physicalDevice(physicalDevice),
	m_surface(surface)
{
}

void LogicalDevice::initialize()
{ 

	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	std::set<uint32_t> uniqueQueueFamilies{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0F;
	for (const auto& queueFamily : uniqueQueueFamilies)
	{
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
			vk::DeviceQueueCreateFlags {},
			queueFamily,
			1,
			&queuePriority
		};

		queueCreateInfos.push_back(deviceQueueCreateInfo);
	}

	auto validationLayers = ValidationLayer::getValidationLayers();

	vk::PhysicalDeviceFeatures deviceFeatures{};
	vk::DeviceCreateInfo createInfo { 
		vk::DeviceCreateFlags {},
		queueCreateInfos,
		validationLayers,
		m_deviceExtensions,
		&deviceFeatures 
	};


	//TODO - Add calidation layers enabled disabled
	if (true)
	{
		createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	m_device = m_physicalDevice.createDevice(createInfo);

	m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
	m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);

}

void LogicalDevice::releaseResources()
{
	m_device.destroy();
}

const vk::Device& LogicalDevice::getDevice() const
{
	return m_device;
}

const vk::Queue& LogicalDevice::getGraphiceQueue() const
{
	return m_graphicsQueue;
}

const vk::Queue& LogicalDevice::getPresentQueue() const
{
	return m_presentQueue;
}




}