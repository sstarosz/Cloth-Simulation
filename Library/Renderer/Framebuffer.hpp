#ifndef RENDERER_FRAMEBUFFER_HPP
#define RENDERER_FRAMEBUFFER_HPP

#include <vulkan/vulkan.hpp>

#include "SwapChain.hpp"
#include "RenderPass/RenderPass.hpp"
#include "VulkanImages/ImageMenager.hpp"
#include <vector>

namespace st::renderer {

class Framebuffer
{

	public:
		Framebuffer(const vk::Device& device,
					const SwapChain& swapChain,
					const RenderPass& renderPass,
					const ImageMenager& imageMenager);

		void initialize();
		void releaseResources();




	private:
		const vk::Device& m_device;
		const SwapChain& m_swapChain;
		const RenderPass& m_renderPass;
		const ImageMenager& m_imageMenager;

		std::vector<vk::Framebuffer> m_swapChainFramebuffers;

		//DepthBuffer
		vk::Image m_depthImage;
		vk::DeviceMemory m_depthImageMemory;
		vk::ImageView m_depthImageView;

		void createDepthResources();
};


};

#endif // RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
