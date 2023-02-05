#include "Framebuffer.hpp"


namespace st::renderer{

	Framebuffer::Framebuffer(const vk::Device& device,
							 const SwapChain& swapChain,
							 const RenderPass& renderPass,
							 const ImageMenager& imageMenager):
	m_device(device),
	m_swapChain(swapChain),
	m_renderPass(renderPass),
	m_imageMenager(imageMenager)
	{
	}

	void Framebuffer::initialize()
	{ 
		createDepthResources();

		auto swapchainImageViews = m_swapChain.getSwapChainImagesViews();
		m_swapChainFramebuffers.reserve(swapchainImageViews.size());

		for (const auto& swapChainImageView : swapchainImageViews)
		{
			std::array<vk::ImageView, 2> attachments{ swapChainImageView, m_depthImageView };

			vk::FramebufferCreateInfo framebufferInfo {
				vk::FramebufferCreateFlags {},
				m_renderPass.getRenderPass(),
				attachments,
				m_swapChain.getSwapchainExtend2D().width,
				m_swapChain.getSwapchainExtend2D().height,
				1
			};

			m_swapChainFramebuffers.emplace_back(m_device.createFramebuffer(framebufferInfo));
		}

	}
	void Framebuffer::createDepthResources()
	{ 
		vk::Format depthFormat = m_renderPass.findDepthFormat();

		vk::Extent2D swapChainExtent = m_swapChain.getSwapchainExtend2D();

		m_imageMenager.createImage(
			swapChainExtent.width, swapChainExtent.height, depthFormat,
			vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
			vk::MemoryPropertyFlagBits::eDeviceLocal, m_depthImage, m_depthImageMemory
		);

		m_depthImageView = createImageView(m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
	}


}

