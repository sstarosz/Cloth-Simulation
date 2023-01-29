#ifndef RENDERER_PHYSICALDEVICE_HPP
#define RENDERER_PHYSICALDEVICE_HPP

#include <vulkan/vulkan.hpp>


namespace st::renderer {


class PhysicalDevice
{
	public:
		PhysicalDevice(const vk::Instance& instance, const vk::SurfaceKHR surface);
		PhysicalDevice(const PhysicalDevice& physicalDevice) = default;

		PhysicalDevice(PhysicalDevice&& physicalDevice) = default;
		PhysicalDevice& operator=(PhysicalDevice&&) = default;

		void initialize();
		void releaseResources();


		vk::PhysicalDevice getPhysicalDevice() const;

	private:
		void pickPhysicalDevice();
		bool isDeviceSuitable(const vk::PhysicalDevice& device);
		bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device);


		const const vk::Instance&   m_instance;
		const const vk::SurfaceKHR& m_surface;


		vk::PhysicalDevice m_physicalDevice;

		constexpr static std::array m_deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};


}


#endif // !RENDERER_PHYSICALDEVICE_HPP