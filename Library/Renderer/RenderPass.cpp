#include "RenderPass.hpp"


namespace st::renderer 
{
RenderPass::RenderPass(const vk::PhysicalDevice& physicalDevice,
					   const vk::Format& surfaceFormat,
					   const vk::Device& device):
	m_physicalDevice(physicalDevice),
	m_surfaceFormat(surfaceFormat),
	m_device(device)
{
}

void RenderPass::initialize()
{

	vk::AttachmentDescription colorAttachment {
		vk::AttachmentDescriptionFlags(), m_surfaceFormat,
		vk::SampleCountFlagBits::e1,      vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,    vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	};

	vk::AttachmentDescription depthAttachment {
		vk::AttachmentDescriptionFlags(),
		findDepthFormat(),
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};


	vk::AttachmentReference colorAttachmentRef { 0, vk::ImageLayout::eColorAttachmentOptimal};
	vk::AttachmentReference depthAttachmentRef { 1, vk::ImageLayout::eDepthStencilAttachmentOptimal};


	vk::SubpassDescription subpass { vk::SubpassDescriptionFlags(),
									 vk::PipelineBindPoint::eGraphics,
									 {},
									 colorAttachmentRef,
									 {},
									 &depthAttachmentRef,
									 {}
	};


	vk::SubpassDependency dependency {
		VK_SUBPASS_EXTERNAL,
		0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::AccessFlagBits::eNoneKHR,
		vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
	};


	std::array<vk::AttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };

	vk::RenderPassCreateInfo renderPassInfo { vk::RenderPassCreateFlags(),
											attachments,
											subpass,
											dependency};


	m_renderPass = m_device.createRenderPass(renderPassInfo);
}

void RenderPass::releaseResources()
{
	m_device.destroyRenderPass(m_renderPass);
}

const vk::RenderPass& RenderPass::getRenderPass() const
{
	return m_renderPass;
}

vk::Format RenderPass::findDepthFormat() const
{
	return findSupportedFormat({ vk::Format::eD32Sfloat,
										  vk::Format::eD32SfloatS8Uint,
										  vk::Format::eD24UnormS8Uint
										 },
										 vk::ImageTiling::eOptimal,
										 vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

vk::Format RenderPass::findSupportedFormat(const std::vector<vk::Format>& candidates,
											vk::ImageTiling tiling,
											vk::FormatFeatureFlags features) const
{
	for (const auto& format : candidates)
	{
		vk::FormatProperties props {
			m_physicalDevice.getFormatProperties(format)
		};

		if (tiling == vk::ImageTiling::eLinear &&
			(props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

}

