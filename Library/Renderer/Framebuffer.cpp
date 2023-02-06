#include "Framebuffer.hpp"


namespace st::renderer 
{

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
		m_swapchainFramebuffers.reserve(swapchainImageViews.size());

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

			m_swapchainFramebuffers.emplace_back(
				m_device.createFramebuffer(framebufferInfo)
			);
		}

	}
	void Framebuffer::releaseResources()
	{
		m_device.destroyImageView(m_depthImageView);
		m_device.destroyImage(m_depthImage);
		m_device.freeMemory(m_depthImageMemory);

		
		for (auto& framebuffer : m_swapchainFramebuffers)
		{
			m_device.destroy(framebuffer);
		}
		m_swapchainFramebuffers.clear();
	}

	const std::vector<vk::Framebuffer> Framebuffer::getSwapchainFramebuffersBuffers() const
	{
		return m_swapchainFramebuffers;
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

		m_depthImageView = m_imageMenager.createImageView(
			m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth
		);
	}


}

