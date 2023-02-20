#ifndef RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
#define RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP

#include <vector>
#include <vulkan/vulkan.hpp>
#include "Renderer/Memory/MemoryManager.hpp"



namespace st::renderer {

class ImageManager
{

	public:
		ImageManager(const vk::Device& device,
			const vk::CommandPool& commandPool,
			const vk::Queue& queue,
			const MemoryManager& memoryMenager);


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

		void transitionImageLayout(vk::Image image,
			                       vk::Format format,
			                       vk::ImageLayout oldLayout,
			                       vk::ImageLayout newLayout);

        void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);


	private:
		vk::CommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

		const vk::Device& m_device;
		const vk::CommandPool& m_commandPool;
		const vk::Queue& m_queue;
		const MemoryManager& m_memoryManager;


};


};

#endif // RENDERER_VULKANIMAGES_IMAGEMENAGER_HPP
