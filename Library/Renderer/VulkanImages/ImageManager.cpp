#include "ImageManager.hpp"


namespace st::renderer
{

	ImageManager::ImageManager(const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& queue, const MemoryManager& memoryMenager):
		m_device(device),
		m_commandPool(commandPool),
		m_queue(queue),
		m_memoryManager(memoryMenager)
	{ }

	void ImageManager::createImage(uint32_t width,
								   uint32_t height,
								   vk::Format format,
								   vk::ImageTiling tiling,
								   vk::ImageUsageFlags usage,
								   vk::MemoryPropertyFlags properties,
								   vk::Image& image,
								   vk::DeviceMemory& imageMemory) const
	{

		vk::ImageCreateInfo imageInfo {
			{},
            vk::ImageType::e2D,          format, { width, height, 1 },
            1, 1, vk::SampleCountFlagBits::e1, tiling,
			usage, vk::SharingMode::eExclusive, {},
            vk::ImageLayout::eUndefined
		};

		image = m_device.createImage(imageInfo);

		vk::MemoryRequirements m_memRequirements = m_device.getImageMemoryRequirements(image);

		vk::MemoryAllocateInfo allocInfo { m_memRequirements.size, m_memoryManager.findMemoryType(m_memRequirements.memoryTypeBits, properties) };

		imageMemory = m_device.allocateMemory(allocInfo);

		m_device.bindImageMemory(image, imageMemory, 0);
	}


	vk::ImageView ImageManager::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
	{
		vk::ImageViewCreateInfo viewInfo {
			{},
            image, vk::ImageViewType::e2D, format, {},
            { aspectFlags, 0, 1, 0, 1 },
            {}
		};

		return m_device.createImageView(viewInfo);
	}


	void ImageManager::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
	{
		vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

		vk::ImageMemoryBarrier barrier {
			vk::AccessFlagBits::eNone, vk::AccessFlagBits::eNone, oldLayout, newLayout,
			VK_QUEUE_FAMILY_IGNORED,   VK_QUEUE_FAMILY_IGNORED,   image,     {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
		};

		vk::PipelineStageFlags sourceStage {};
		vk::PipelineStageFlags destinationStage {};

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
		{
			barrier.srcAccessMask = vk::AccessFlagBits::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
		{
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}
		else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
		{
			barrier.srcAccessMask = vk::AccessFlagBits::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
		}
		else
		{
			throw std::invalid_argument("unsupported layout transition!");
		}

		commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, {}, barrier);

		endSingleTimeCommands(commandBuffer);
	}


	void ImageManager::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
	{
		vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

		vk::BufferImageCopy region {
			0, 0, 0, { vk::ImageAspectFlagBits::eColor, 0, 0, 1 },
               { 0, 0, 0 },
               { width, height, 1 }
		};

		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

		endSingleTimeCommands(commandBuffer);
	}


	vk::CommandBuffer ImageManager::beginSingleTimeCommands()
	{

		vk::CommandBufferAllocateInfo allocInfo { m_commandPool, vk::CommandBufferLevel::ePrimary, 1 };

		vk::CommandBuffer commandBuffer = m_device.allocateCommandBuffers(allocInfo).front();

		vk::CommandBufferBeginInfo beginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

		commandBuffer.begin(beginInfo);
		return commandBuffer;
	}


	void ImageManager::endSingleTimeCommands(vk::CommandBuffer commandBuffer)
	{
		commandBuffer.end();

		vk::SubmitInfo submitInfo { {}, {}, commandBuffer };

		m_queue.submit(submitInfo);
		m_queue.waitIdle();

		m_device.freeCommandBuffers(m_commandPool, commandBuffer);
	}

}
