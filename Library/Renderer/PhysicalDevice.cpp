#include "PhysicalDevice.hpp"
#include "QueueFamily.hpp"
#include "SwapChainSupport.hpp"

#include <set>
#include <string>

namespace st::renderer 
{

	PhysicalDevice::PhysicalDevice(const vk::Instance& instance,
								  const vk::SurfaceKHR& surface):
	m_instance(instance),
	m_surface(surface)
	{
	}

	void PhysicalDevice::initialize()
	{
		pickPhysicalDevice();
	}

	void PhysicalDevice::releaseResources()
	{
		//TODO- nothing??
	}

	//Return copy of pointer
	const vk::PhysicalDevice& PhysicalDevice::getPhysicalDevice() const
	{
		return m_physicalDevice;
	}

	void PhysicalDevice::pickPhysicalDevice()
	{
		std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();

		if (devices.empty())
		{
			throw std::runtime_error("Failed to find GPU's with Vulkan support!");
		}

		for (const auto& device : devices)
		{
			// Check if devices contain all required functionality
			//Pick first that fullfil condtion
			if (isDeviceSuitable(device))
			{
				m_physicalDevice = device;
				break;
			}
		}
	}

	bool PhysicalDevice::isDeviceSuitable(const vk::PhysicalDevice& device)
	{
		//TODO - Check if this function prefere real GPU than gpu inside cpu

		QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, m_surface);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(device, m_surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(const vk::PhysicalDevice& device)
	{
		const std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}


}
