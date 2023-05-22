#include "SwapChain.hpp"

#include "SwapChainSupport.hpp"
#include "QueueFamily.hpp"

namespace st::renderer
{

	SwapChain::SwapChain(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Device& device):
		m_physicalDevice(physicalDevice),
		m_surface(surface),
		m_device(device)
	{ }

	void SwapChain::initialize()
	{
		SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(m_physicalDevice, m_surface);

		const vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		const vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		const vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		//TODO - Image count == 2?
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(m_physicalDevice, m_surface);

		std::array<uint32_t, 2> queueFamilyIndices { indices.graphicsFamily.value(), indices.presentFamily.value() };

		const vk::SharingMode imageSharingMode = (indices.graphicsFamily != indices.presentFamily) ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;


		const vk::SwapchainCreateInfoKHR createInfo { vk::SwapchainCreateFlagsKHR(),
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
												VK_NULL_HANDLE };

		m_swapChain = m_device.createSwapchainKHR(createInfo);
		m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		createImageViews();
	}

	void SwapChain::releaseResources()
	{

		for (auto& imageView : m_swapChainImageViews)
		{
			m_device.destroyImageView(imageView);
		}
		m_swapChainImageViews.clear();

		m_device.destroySwapchainKHR(m_swapChain);
	}

	void SwapChain::updateSwapChain(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
	}

	void SwapChain::recreateSwapChain(uint32_t width, uint32_t height)
	{
		updateSwapChain(width, height);
		releaseResources();
		initialize();
	}

	const vk::SwapchainKHR& SwapChain::getSwapchain() const noexcept
	{
		return m_swapChain;
	}

	const vk::Extent2D& SwapChain::getSwapchainExtend2D() const noexcept
	{
		return m_swapChainExtent;
	}

	const vk::Format& SwapChain::getSwapChainImageFormat() const noexcept
	{
		return m_swapChainImageFormat;
	}

	const std::vector<vk::Image>& SwapChain::getSwapChainImages() const noexcept
	{
		return m_swapChainImages;
	}

	const std::vector<vk::ImageView>& SwapChain::getSwapChainImagesViews() const noexcept
	{
		return m_swapChainImageViews;
	}

	vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
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

		return vk::PresentModeKHR { VK_PRESENT_MODE_FIFO_KHR };
	}

	vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent { m_width, m_width };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void SwapChain::createImageViews()
	{
		for (const auto& swapChainImage : m_swapChainImages)
		{
			const vk::ImageViewCreateInfo createInfo {
				vk::ImageViewCreateFlags {},
				swapChainImage,
				vk::ImageViewType::e2D,
				m_swapChainImageFormat,
				vk::ComponentMapping { vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity,
										  vk::ComponentSwizzle::eIdentity },
				vk::ImageSubresourceRange { vk::ImageAspectFlags { vk::ImageAspectFlagBits::eColor }, 0, 1, 0, 1 }
			};

			m_swapChainImageViews.emplace_back(m_device.createImageView(createInfo));
		}
	}

}