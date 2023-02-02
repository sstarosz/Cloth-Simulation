#include "SwapChain.hpp"

#include "SwapChainSupport.hpp"
#include "QueueFamily.hpp"

namespace st::renderer {
SwapChain::SwapChain(
	const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
	const vk::Device& device
):
	m_physicalDevice(physicalDevice),
	m_surface(surface),
	m_device(device)
{
}

void SwapChain::initialize()
{
	SwapChainSupportDetails swapChainSupport
		= SwapChainSupportDetails::querySwapChainSupport(m_physicalDevice, m_surface);

	vk::SurfaceFormatKHR surfaceFormat
		= chooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0
		&& imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	QueueFamilyIndices indices
		= QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);

	std::array<uint32_t, 2> queueFamilyIndices { indices.graphicsFamily.value(),
												 indices.presentFamily.value() };

	vk::SharingMode imageSharingMode = (indices.graphicsFamily != indices.presentFamily)
		? vk::SharingMode::eConcurrent
		: vk::SharingMode::eExclusive;


	vk::SwapchainCreateInfoKHR createInfo {
		vk::SwapchainCreateFlagsKHR(),
		m_surface,
		imageCount,
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		extent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		imageSharingMode,
		queueFamilyIndices,
		swapChainSupport.capabilities.currentTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		presentMode,
		VK_TRUE,
		VK_NULL_HANDLE
	};

	m_swapChain = m_device.createSwapchainKHR(createInfo);
	m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;
}

void SwapChain::releaseResources()
{
	m_device.destroySwapchainKHR(m_swapChain);
}

void SwapChain::updateSwapChain(uint64_t width, uint64_t height)
{
	m_width = width;
	m_height = height;
}

void SwapChain::recreateSwapChain(uint64_t width, uint64_t height)
{
	updateSwapChain(width, height);
	releaseResources();
	initialize();
}

const vk::SwapchainKHR& SwapChain::getSwapchain() const
{
	return m_swapChain;
}

const vk::Extent2D& SwapChain::getSwapchainExtend2D() const
{
	return m_swapChainExtent;
}

const vk::Format& SwapChain::getSwapChainImageFormat() const
{
	return m_swapChainImageFormat;
}

const std::vector<vk::Image>& SwapChain::getSwapChainImages() const
{
	return m_swapChainImages;
}

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == vk::Format::eB8G8R8A8Srgb
			&& availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == vk::PresentModeKHR::eMailbox)
		{
			return availablePresentMode;
		}
	}

	return vk::PresentModeKHR(VK_PRESENT_MODE_FIFO_KHR);
}

vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width = m_width;
		int height = m_height;

		VkExtent2D actualExtent { static_cast<uint32_t>(width),
								  static_cast<uint32_t>(height) };

		actualExtent.width = std::clamp(
			actualExtent.width, capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		actualExtent.height = std::clamp(
			actualExtent.height, capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);

		return actualExtent;
	}
}

}