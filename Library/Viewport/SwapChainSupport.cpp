#include "SwapChainSupport.hpp"


namespace st::viewport
{
	SwapChainSupportDetails SwapChainSupportDetails::querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
		SwapChainSupportDetails details;

		details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
		details.formats = device.getSurfaceFormatsKHR(surface);
		details.presentModes = device.getSurfacePresentModesKHR(surface);

		return details;
	}

}

