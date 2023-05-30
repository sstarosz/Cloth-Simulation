#ifndef RENDERER_SWAPCHAIN_HPP
#define RENDERER_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>

namespace st::renderer
{


	class SwapChain
	{
	public:
		SwapChain(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Device& device) noexcept;


		void initialize();
		void releaseResources();

		void updateSwapChain(uint32_t width, uint32_t height);
		void recreateSwapChain(uint32_t width, uint32_t height);


		const vk::SwapchainKHR& getSwapchain() const noexcept;
		const vk::Extent2D& getSwapchainExtend2D() const noexcept;
		const vk::Format& getSwapChainImageFormat() const noexcept;
		const std::vector<vk::Image>& getSwapChainImages() const noexcept;

		const std::vector<vk::ImageView>& getSwapChainImagesViews() const noexcept;

	private:
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const noexcept;
		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const noexcept;
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const noexcept;

		void createImageViews();



		const vk::PhysicalDevice& m_physicalDevice;
		const vk::SurfaceKHR& m_surface;
		const vk::Device& m_device;


		vk::SwapchainKHR m_swapChain;
		std::vector<vk::Image> m_swapChainImages;
		vk::Format m_swapChainImageFormat;
		vk::Extent2D m_swapChainExtent;
		std::vector<vk::ImageView> m_swapChainImageViews;
	
		uint32_t m_width;
		uint32_t m_height;
	};


}
#endif // RENDERER_SWAPCHAIN_HPP