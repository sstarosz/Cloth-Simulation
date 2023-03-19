#ifndef RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP
#define RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "Renderer/Memory/MemoryManager.hpp"


namespace st::renderer
{


	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(const vk::PhysicalDevice& physicalDevice,
						 const vk::Device& device,
						 const vk::RenderPass& renderPass,
						 const MemoryManager& memoryMenager);


		void initialize();
		void releaseResources();

		const vk::Pipeline& getGraphicsPipeline() const;
		const vk::PipelineLayout& getPipelineLayout() const;
		const vk::DescriptorSet& getDescriptorSet(uint32_t currentFrame) const;
		const vk::DeviceMemory& getUniformBufferMemory(uint32_t currentFrame) const;

		const std::vector<vk::DescriptorSet> createDescriptorSet() const;
		const void updateDescriptorSet(const vk::ImageView& imageView);

	private:
		void createTextureSampler();
		void createUniformBuffers();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();


		const vk::PhysicalDevice& m_physicalDevice;
		const vk::Device& m_device;
		const vk::RenderPass& m_renderPass;
		const MemoryManager& m_memoryMenager;


		vk::Pipeline m_graphicsPipeline;
		vk::PipelineLayout m_pipelineLayout;
		vk::PipelineCache m_pipelineCache;
		std::vector<vk::DynamicState> m_dynamicStateEnables;
		vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;


		vk::DescriptorPool m_primitiveDescriptorPool;
		std::vector<vk::DescriptorSet> m_primitiveDescriptorSets;
		vk::DescriptorSetLayout m_descriptorSetLayout;

		std::vector<vk::Buffer> m_uniformBuffers;
		std::vector<vk::DeviceMemory> m_uniformBuffersMemory;

		vk::Sampler m_textureSampler;


		const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	};


}
#endif // RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP