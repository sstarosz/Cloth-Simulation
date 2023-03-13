#ifndef RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP
#define RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>


namespace st::renderer
{


	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::RenderPass& renderPass);


		void initialize();
		void releaseResources();

		const vk::Pipeline& getGraphicsPipeline() const;
		const vk::PipelineLayout& getPipelineLayout() const;
		const vk::DescriptorSetLayout& getDescriptorSetLayout() const;


	private:
		void createDescriptorSetLayout();


		const vk::PhysicalDevice& m_physicalDevice;
		const vk::Device& m_device;
		const vk::RenderPass& m_renderPass;


		vk::Pipeline m_graphicsPipeline;
		vk::PipelineLayout m_pipelineLayout;
		vk::PipelineCache m_pipelineCache;
		std::vector<vk::DynamicState> m_dynamicStateEnables;
		vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;

		vk::DescriptorSetLayout m_descriptorSetLayout;
	};


}
#endif // RENDERER_PIPELINE_GRAPHICSPIPELINE_HPP