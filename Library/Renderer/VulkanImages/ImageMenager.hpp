#ifndef RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
#define RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP

#include <vector>
#include <vulkan/vulkan.hpp>
#include "Renderer/Memory/MemoryManager.hpp"



namespace st::renderer {

class ImageMenager
{

	public:
		ImageMenager(const vk::Device& device, const MemoryManager& memoryMenager);

		void initialize();
		void releaseResources();



		void createImage(uint32_t width, uint32_t height,
						 vk::Format format,
						 vk::ImageTiling tiling,
						 vk::ImageUsageFlags usage,
						 vk::MemoryPropertyFlags properties,
						 vk::Image& image,
						 vk::DeviceMemory& imageMemory) const;

		vk::ImageView createImageView(vk::Image image,
									  vk::Format format,
									  vk::ImageAspectFlags aspectFlags) const;

	private:
		const vk::Device& m_device;
		const MemoryManager& m_memoryManager;


};


};

#endif // RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
