#ifndef VIEWPORT_SWAPCHAINSUPPORT_HPP
#define VIEWPORT_SWAPCHAINSUPPORT_HPP


#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::viewport
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;

		static SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
	};

};

#endif // VIEWPORT_SWAPCHAINSUPPORT_HPP
