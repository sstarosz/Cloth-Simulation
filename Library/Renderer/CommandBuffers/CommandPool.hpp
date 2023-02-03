#ifndef RENDERER_COMMANDBUFFERS_COMMANDPOOL_HPP
#define RENDERER_COMMANDBUFFERS_COMMANDPOOL_HPP

#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::renderer {

class CommandPool
{

	public:
		CommandPool(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

		void initialize();
		void releaseResources();

		const vk::CommandPool& getCommandPool() const;

	private:
		const vk::Device& m_device;
		const vk::PhysicalDevice& m_physicalDevice;
		const vk::SurfaceKHR& m_surfac;

		vk::CommandPool m_commandPool;
};


};

#endif // RENDERER_SHADERS_SHADERS_HPP
