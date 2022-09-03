
#ifndef VULKANWINDOW_HPP
#define VULKANWINDOW_HPP


#include "Config.hpp"
#include "vulkan/vulkan.hpp"
#include <QWindow>
#include <QVulkanInstance>
#include <array>
#include <string_view>
#include <Geometry/Vertex.hpp>

namespace st::viewport 
{
	class VulkanWindow : public QWindow
	{
	Q_OBJECT

	public:
		VulkanWindow();


		void initialize();
		void releaseResources();

	private:
		QVulkanInstance inst;

		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
		vk::SurfaceKHR m_surface;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		vk::SwapchainKHR m_swapChain;
		std::vector<vk::Image> m_swapChainImages;
		vk::Format m_swapChainImageFormat;
		vk::Extent2D m_swapChainExtent;

		std::vector<vk::ImageView> m_swapChainImageViews;

		vk::RenderPass m_renderPass;

		vk::DescriptorSetLayout m_descriptorSetLayout;
		vk::PipelineLayout m_pipelineLayout;
		vk::PipelineCache m_pipelineCache;
		vk::Pipeline m_graphicsPipeline;

		std::vector<vk::Framebuffer> m_swapChainFramebuffers;

		vk::CommandPool m_commandPool;

		vk::Buffer m_vertexBuffer;
		vk::DeviceMemory m_vertexBufferMemory;
		vk::Buffer m_indexBuffer;
		vk::DeviceMemory m_indexBufferMemory;

		constexpr static std::array validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		constexpr static std::array deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);


		/*To Delete*/
		const std::vector<geometry::Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
		};

		#ifdef NDEBUG
				const bool enableValidationLayers = false;
		#else
				const bool enableValidationLayers = true;
		#endif


		/*Init*/
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();

		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffers();
		void createDescriptorPool();
		void createDescriptorSets();
		void createCommandBuffers();
		void createSyncObjects();

		/*Hellper function*/
		bool isDeviceSuitable(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device);
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

	};

} //!namespace st::viewport
#endif // !VULKANWINDOW_HPP
