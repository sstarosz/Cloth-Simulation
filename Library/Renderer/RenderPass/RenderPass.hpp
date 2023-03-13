#ifndef RENDERER_RENDERPASS_HPP
#define RENDERER_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


namespace st::renderer
{


	class RenderPass
	{
	public:
		RenderPass(const vk::PhysicalDevice& physicalDevice, const vk::Format& surfaceFormat, const vk::Device& device);


		void initialize();
		void releaseResources();

		const vk::RenderPass& getRenderPass() const;
		vk::Format findDepthFormat() const;
		vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;

	private:
		const vk::PhysicalDevice& m_physicalDevice;
		const vk::Format& m_surfaceFormat;
		const vk::Device& m_device;


		vk::RenderPass m_renderPass;

		vk::ImageView m_depthImageView;
	};


}
#endif // RENDERER_SWAPCHAIN_HPP