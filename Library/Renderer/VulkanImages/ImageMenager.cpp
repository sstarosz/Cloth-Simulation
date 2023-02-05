#include "ImageMenager.hpp"



namespace st::renderer
{

ImageMenager::ImageMenager(const vk::Device& device, const MemoryManager& memoryManager):
	m_device(device),
	m_memoryManager(memoryManager)
	{ 
	}

	vk::ImageView ImageMenager::createImage(uint32_t width,
											uint32_t height,
											vk::Format format,
											vk::ImageTiling tiling,
											vk::ImageUsageFlags usage,
											vk::MemoryPropertyFlags properties,
											vk::Image& image,
											vk::DeviceMemory& imageMemory)
	{
		vk::ImageCreateInfo imageInfo {
			{},
			vk::ImageType::e2D,
			format,
			{ width, height, 1 },
			1,
			1,
			vk::SampleCountFlagBits::e1,
			tiling,
			usage,
			vk::SharingMode::eExclusive,
			{},
			vk::ImageLayout::eUndefined
		};

		image = m_device.createImage(imageInfo);

		vk::MemoryRequirements m_memRequirements = m_device.getImageMemoryRequirements(image);

		vk::MemoryAllocateInfo allocInfo {
			m_memRequirements.size,
			m_memoryManager.findMemoryType(m_memRequirements.memoryTypeBits, properties)
		};

		imageMemory = m_device.allocateMemory(allocInfo);

		m_device.bindImageMemory(image, imageMemory, 0);
	}

}

