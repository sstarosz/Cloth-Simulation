#include "PrimitivesGraphicsPipeline.hpp"

#include "Renderer/Shaders/Shader.hpp"
#include "Geometry/Line.hpp"
#include "Renderer/UniformBuffers.hpp"

namespace st::renderer
{

	PrimitivesGraphicsPipeline::PrimitivesGraphicsPipeline(const vk::PhysicalDevice& physicalDevice,
														   const vk::Device& device,
														   const vk::RenderPass& renderPass,
														   const MemoryManager& memoryMenager):
		m_physicalDevice(physicalDevice),
		m_device(device),
		m_renderPass(renderPass),
		m_memoryMenager(memoryMenager)
	{ }

	void PrimitivesGraphicsPipeline::initialize()
	{
		createUniformBuffers();
		createDescriptorPool();
		createDescriptorSetLayout();
		createDescriptorSets();


		auto vertShaderCode = Shader::readFile("../Assets/Shaders/line_vert.spv");
		auto fragShaderCode = Shader::readFile("../Assets/Shaders/line_frag.spv");


		vk::ShaderModule vertShaderModule = Shader::createShaderModule(m_device, vertShaderCode);
		vk::ShaderModule fragShaderModule = Shader::createShaderModule(m_device, fragShaderCode);


		vk::PipelineShaderStageCreateInfo vertShaderStageInfo { {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main" };

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo { {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main" };

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertShaderStageInfo, fragShaderStageInfo };


		auto bindingDescription = geometry::Line::getBindingDescription();
		auto attributeDescriptions = geometry::Line::getAttributeDescriptions();

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo { {}, bindingDescription, attributeDescriptions };

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly { vk::PipelineInputAssemblyStateCreateFlags {}, vk::PrimitiveTopology::eLineList, VK_FALSE };


		vk::PipelineViewportStateCreateInfo viewportState { vk::PipelineViewportStateCreateFlags {}, 1, {}, 1, {} };

		vk::PipelineRasterizationStateCreateInfo rasterizer { vk::PipelineRasterizationStateCreateFlags {},
															  VK_FALSE,
															  VK_FALSE,
															  vk::PolygonMode::eFill,
															  vk::CullModeFlagBits::eNone,
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
		m_primitivesGraphicsPipeline = m_device.createGraphicsPipeline(m_pipelineCache, pipelineInfo).value;


		//Note VkShaderModule is passed into pipeline and are not longer available thought object they are used to create
		//If there are used later, then they must not be destroyed
		m_device.destroy(fragShaderModule);
		m_device.destroy(vertShaderModule);
	}

	void PrimitivesGraphicsPipeline::releaseResources()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_device.destroyBuffer(m_uniformBuffers[i]);
			m_device.freeMemory(m_uniformBuffersMemory[i]);
		}


		m_device.destroyDescriptorPool(m_primitiveDescriptorPool);

		m_device.destroyDescriptorSetLayout(m_descriptorSetLayout);

		m_device.destroyPipelineCache(m_pipelineCache);
		m_device.destroyPipeline(m_primitivesGraphicsPipeline);
		m_device.destroyPipelineLayout(m_pipelineLayout);
	}

	const vk::Pipeline& PrimitivesGraphicsPipeline::getGraphicsPipeline() const
	{
		return m_primitivesGraphicsPipeline;
	}

	const vk::PipelineLayout& PrimitivesGraphicsPipeline::getPipelineLayout() const
	{
		return m_pipelineLayout;
	}


	const vk::DescriptorSet& PrimitivesGraphicsPipeline::getDescriptorSet(uint32_t currentFrame) const
	{
		return m_primitiveDescriptorSets.at(currentFrame);
	}

	const vk::DeviceMemory& PrimitivesGraphicsPipeline::getUniformBufferMemory(uint32_t currentFrame) const
	{
		return m_uniformBuffersMemory.at(currentFrame);
	}

	void PrimitivesGraphicsPipeline::createUniformBuffers()
	{
		const VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_memoryMenager.createBuffer(bufferSize,
										 vk::BufferUsageFlagBits::eUniformBuffer,
										 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
										 m_uniformBuffers[i],
										 m_uniformBuffersMemory[i]);
		}
	}

	void PrimitivesGraphicsPipeline::createDescriptorSetLayout()
	{
		vk::DescriptorSetLayoutBinding uboLayoutBinding { 0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex };


		std::array<vk::DescriptorSetLayoutBinding, 1> bindings { uboLayoutBinding };
		vk::DescriptorSetLayoutCreateInfo layoutInfo { {}, bindings };

		m_descriptorSetLayout = m_device.createDescriptorSetLayout(layoutInfo);
	}

	void PrimitivesGraphicsPipeline::createDescriptorPool() 
	{
		vk::DescriptorPoolSize poolsSize { vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT };

		vk::DescriptorPoolCreateInfo poolInfo { {}, MAX_FRAMES_IN_FLIGHT, poolsSize };

		m_primitiveDescriptorPool = m_device.createDescriptorPool(poolInfo);
	}

	void PrimitivesGraphicsPipeline::createDescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> primitiveLayouts(MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);

		vk::DescriptorSetAllocateInfo graphicAllocInfo { m_primitiveDescriptorPool, primitiveLayouts };


		m_primitiveDescriptorSets = m_device.allocateDescriptorSets(graphicAllocInfo);


		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vk::DescriptorBufferInfo bufferInfo { m_uniformBuffers.at(i), 0, sizeof(UniformBufferObject) };

			std::array<vk::WriteDescriptorSet, 1> primitivesDescriptorWrites {
				vk::WriteDescriptorSet {m_primitiveDescriptorSets.at(i), 0, 0, vk::DescriptorType::eUniformBuffer, {}, bufferInfo, {}}
			};

			m_device.updateDescriptorSets(primitivesDescriptorWrites, {});
		}
	}

}
