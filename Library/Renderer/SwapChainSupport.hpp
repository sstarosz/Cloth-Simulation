#ifndef RENDERER_SWAPCHAINSUPPORT_HPP
#define RENDERER_SWAPCHAINSUPPORT_HPP


#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::renderer
{
	struct SwapChainSupportDetails
	{
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
		vk::SurfaceCapabilitiesKHR capabilities;

		static SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
	};

};

#endif // RENDERER_SWAPCHAINSUPPORT_HPP
