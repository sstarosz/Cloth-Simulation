
#ifndef VULKANWINDOW_HPP
#define VULKANWINDOW_HPP


#include "Config.hpp"
#include "vulkan/vulkan.hpp"
#include <QWindow>
#include <QVulkanInstance>
#include <array>
#include <string_view>
#include <Geometry/Vertex.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace st::viewport 
{
	class VulkanWindow : public QWindow
	{
	Q_OBJECT

	public:
		VulkanWindow();


		void initialize();
		void releaseResources();

	protected:
		void exposeEvent(QExposeEvent*) override;
		void resizeEvent(QResizeEvent*) override;
		bool event(QEvent* ev) override;

	private slots:
		void update();

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

		std::vector<vk::Buffer> m_uniformBuffers;
		std::vector<vk::DeviceMemory> m_uniformBuffersMemory;

		vk::DescriptorPool m_descriptorPool;
		std::vector<vk::DescriptorSet> m_descriptorSets;

		std::vector<vk::CommandBuffer> m_commandBuffers;

		std::vector<vk::Semaphore> m_imageAvailableSemaphores;
		std::vector<vk::Semaphore> m_renderFinishedSemaphores;
		std::vector<vk::Fence> m_inFlightFences;


		const static uint32_t MAX_FRAMES_IN_FLIGHT = 2;

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


		//struct UniformBufferObject {
		//	geometry::mat4 model;
		//	geometry::mat4 view;
		//	geometry::mat4 proj;
		//};

		struct UniformBufferObject {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};


		uint32_t currentFrame = 0;
		bool m_framebufferResized = false;

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


		void updateUniformBuffer(uint32_t currentImage);
		void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
		void drawFrame();

		/*Clean up*/
		void cleanupSwapChain();
		void recreateSwapChain();

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
