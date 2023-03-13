#ifndef RENDERER_SWAPCHAIN_HPP
#define RENDERER_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>

namespace st::renderer
{


	class SwapChain
	{
	public:
		SwapChain(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Device& device);


		void initialize();
		void releaseResources();

		void updateSwapChain(uint64_t width, uint64_t height);
		void recreateSwapChain(uint64_t width, uint64_t height);


		const vk::SwapchainKHR& getSwapchain() const;
		const vk::Extent2D& getSwapchainExtend2D() const;
		const vk::Format& getSwapChainImageFormat() const;
		const std::vector<vk::Image>& getSwapChainImages() const;

		const std::vector<vk::ImageView>& getSwapChainImagesViews() const;

	private:
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;
		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const;
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

		void createImageViews();


		uint64_t m_width;
		uint64_t m_height;

		const vk::PhysicalDevice& m_physicalDevice;
		const vk::SurfaceKHR& m_surface;
		const vk::Device& m_device;


		vk::SwapchainKHR m_swapChain;
		std::vector<vk::Image> m_swapChainImages;
		vk::Format m_swapChainImageFormat;
		vk::Extent2D m_swapChainExtent;
		std::vector<vk::ImageView> m_swapChainImageViews;
	};


}
#endif // RENDERER_SWAPCHAIN_HPP