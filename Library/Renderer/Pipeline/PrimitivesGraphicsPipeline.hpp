#ifndef RENDERER_PIPELINE_PRIMITIVESGRAPHICSPIPELINE_HPP
#define RENDERER_PIPELINE_PRIMITIVESGRAPHICSPIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "Renderer/Memory/MemoryManager.hpp"


namespace st::renderer
{

	class PrimitivesGraphicsPipeline
	{
	public:
		PrimitivesGraphicsPipeline(const vk::PhysicalDevice& physicalDevice,
								   const vk::Device& device,
								   const vk::RenderPass& renderPass,
								   const MemoryManager& memoryMenager);


		void initialize();
		void releaseResources();

		const vk::Pipeline& getGraphicsPipeline() const;
		const vk::PipelineLayout& getPipelineLayout() const;
		const vk::DescriptorSet& getDescriptorSet(uint32_t currentFrame) const;
		const vk::DeviceMemory& getUniformBufferMemory(uint32_t currentFrame) const;

	private:
		void createUniformBuffers();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createDescriptorSets();

		const vk::PhysicalDevice& m_physicalDevice;
		const vk::Device& m_device;
		const vk::RenderPass& m_renderPass;
		const MemoryManager& m_memoryMenager;


		vk::Pipeline m_primitivesGraphicsPipeline;
		vk::PipelineLayout m_pipelineLayout;
		vk::PipelineCache m_pipelineCache;
		std::vector<vk::DynamicState> m_dynamicStateEnables;
		vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;


		vk::DescriptorPool m_primitiveDescriptorPool;
		std::vector<vk::DescriptorSet> m_primitiveDescriptorSets;
		vk::DescriptorSetLayout m_descriptorSetLayout;

		std::vector<vk::Buffer> m_uniformBuffers;
		std::vector<vk::DeviceMemory> m_uniformBuffersMemory;

		const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	};


}
#endif // RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP