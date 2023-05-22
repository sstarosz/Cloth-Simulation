#ifndef RENDERER_PHYSICALDEVICE_HPP
#define RENDERER_PHYSICALDEVICE_HPP

#include <vulkan/vulkan.hpp>


namespace st::renderer
{

	class PhysicalDevice
	{
	public:
		PhysicalDevice(const vk::Instance& instance, const vk::SurfaceKHR& surface) noexcept;

		void initialize();
		void releaseResources() noexcept;


		const vk::PhysicalDevice& getPhysicalDevice() const noexcept;

	private:
		void pickPhysicalDevice();
		bool isDeviceSuitable(const vk::PhysicalDevice& device);
		bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device);


		const vk::Instance& m_instance;
		const vk::SurfaceKHR& m_surface;


		vk::PhysicalDevice m_physicalDevice;

		constexpr static std::array m_deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};


}


#endif // !RENDERER_PHYSICALDEVICE_HPP