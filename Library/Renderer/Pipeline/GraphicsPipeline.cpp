#include "GraphicsPipeline.hpp"

#include "Renderer/Shaders/Shader.hpp"
#include "Geometry/Vertex.hpp"

namespace st::renderer
{

	GraphicsPipeline::GraphicsPipeline(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::RenderPass& renderPass):
		m_physicalDevice(physicalDevice),
		m_device(device),
		m_renderPass(renderPass)
	{ }

	void GraphicsPipeline::initialize()
	{
		createDescriptorSetLayout();

		auto vertShaderCode = Shader::readFile("../Assets/Shaders/vert.spv");
		auto fragShaderCode = Shader::readFile("../Assets/Shaders/frag.spv");


		vk::ShaderModule vertShaderModule = Shader::createShaderModule(m_device, vertShaderCode);
		vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);


		vk::PipelineShaderStageCreateInfo vertShaderStageInfo { {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main" };

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo { {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main" };

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };


		auto bindingDescription = geometry::Vertex::getBindingDescription();
		auto attributeDescriptions = geometry::Vertex::getAttributeDescriptions();

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo { {}, bindingDescription, attributeDescriptions };

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly { vk::PipelineInputAssemblyStateCreateFlags {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE };


		vk::PipelineViewportStateCreateInfo viewportState { vk::PipelineViewportStateCreateFlags {}, 1, {}, 1, {} };

		vk::PipelineRasterizationStateCreateInfo rasterizer { vk::PipelineRasterizationStateCreateFlags {},
															  VK_FALSE,
															  VK_FALSE,
															  vk::PolygonMode::eFill,
															  vk::CullModeFlagBits::eBack,
															  vk::FrontFace::eCounterClockwise,
															  VK_FALSE,
															  0.0f,
															  0.0f,
															  0.0f,
															  1.0F };

		vk::PipelineMultisampleStateCreateInfo multisampling { vk::PipelineMultisampleStateCreateFlags {}, vk::SampleCountFlagBits::e1, VK_FALSE };

		vk::PipelineDepthStencilStateCreateInfo depthStencil { {}, true, true, vk::CompareOp::eLess, false, false };

		m_dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

		m_pipelineDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo { {}, m_dynamicStateEnables };

		vk::PipelineColorBlendAttachmentState colorBlendAttachment { VK_FALSE,
																	 vk::BlendFactor::eZero,
																	 vk::BlendFactor::eZero,
																	 vk::BlendOp::eAdd,
																	 vk::BlendFactor::eZero,
																	 vk::BlendFactor::eZero,
																	 vk::BlendOp::eAdd,
																	 vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
																		 vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };


		vk::PipelineColorBlendStateCreateInfo colorBlending {
			vk::PipelineColorBlendStateCreateFlags {},
			VK_FALSE,
			vk::LogicOp::eCopy,
			colorBlendAttachment,
			{ 0.0f, 0.0f, 0.0f, 0.0f }
		};


		vk::PipelineLayoutCreateInfo pipelineLayoutInfo { vk::PipelineLayoutCreateFlags {}, m_descriptorSetLayout };

		m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

		vk::GraphicsPipelineCreateInfo pipelineInfo { vk::PipelineCreateFlags {},
													  shaderStages,
													  &vertexInputInfo,
													  &inputAssembly,
													  {},
													  &viewportState,
													  &rasterizer,
													  &multisampling,
													  &depthStencil,
													  &colorBlending,
													  &m_pipelineDynamicStateCreateInfo,
													  m_pipelineLayout,
													  m_renderPass };


		m_pipelineCache = m_device.createPipelineCache(vk::PipelineCacheCreateInfo());
		m_graphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;


		//Note VkShaderModule is passed into pipline and are not longer available trought object they are used to create
		//If ther are used later, then they must not be destroyed
		m_device.destroy(fragShaderModule);
		m_device.destroy(vertShaderModule);
	}

	void GraphicsPipeline::releaseResources()
	{
		m_device.destroyDescriptorSetLayout(m_descriptorSetLayout);

		m_device.destroyPipelineCache(m_pipelineCache);
		m_device.destroyPipeline(m_graphicsPipeline);
		m_device.destroyPipelineLayout(m_pipelineLayout);
	}

	const vk::Pipeline& GraphicsPipeline::getGraphicsPipeline() const
	{
		return m_graphicsPipeline;
	}

	const vk::PipelineLayout& GraphicsPipeline::getPipelineLayout() const
	{
		return m_pipelineLayout;
	}

	const vk::DescriptorSetLayout& GraphicsPipeline::getDescriptorSetLayout() const
	{
		return m_descriptorSetLayout;
	}

	void GraphicsPipeline::createDescriptorSetLayout()
	{
		vk::DescriptorSetLayoutBinding uboLayoutBinding { 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex };

		vk::DescriptorSetLayoutBinding samplerLayoutBinding { 1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment };

		std::array<vk::DescriptorSetLayoutBinding, 2> bindings { uboLayoutBinding, samplerLayoutBinding };
		vk::DescriptorSetLayoutCreateInfo layoutInfo { {}, bindings };

		m_descriptorSetLayout = m_device.createDescriptorSetLayout(layoutInfo);
	}

}
