#ifndef RENDERER_LOGICALDEVICE_HPP
#define RENDERER_LOGICALDEVICE_HPP

#include <vulkan/vulkan.hpp>

#include <array>

namespace st::renderer {


class LogicalDevice
{
	public:
		LogicalDevice(const vk::Instance& instance,
			const vk::PhysicalDevice& physicalDevice,
			const vk::SurfaceKHR& surface);

		void initialize();
		void releaseResources();


		const vk::Device& getDevice() const;
		const vk::Queue& getGraphiceQueue() const;
		const vk::Queue& getPresentQueue() const;


	private:
		const vk::Instance& m_instance;
		const vk::PhysicalDevice& m_physicalDevice;
		const vk::SurfaceKHR& m_surface;


		vk::Device m_device;

		//TODO - Move it to other place
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		constexpr static std::array m_deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};


}


#endif // !RENDERER_LOGICALDEVICE_HPP