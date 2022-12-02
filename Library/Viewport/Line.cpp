#include "Line.hpp"
#include "Shader.hpp"

namespace st::viewport
{



    vk::RenderPassBeginInfo Line::getRenderPass(vk::Framebuffer frameBuffer, vk::Extent2D swapChainExtent) const
    {
        const vk::ClearColorValue colorClean{ std::array<float, 4>{0.0F, 0.0F, 0.0F, 1.0F} };
        const vk::ClearDepthStencilValue depthClean{ 1.0F, 0 };

        std::array<vk::ClearValue, 2> clearValues;
        clearValues[0].setColor(colorClean);
        clearValues[1].setDepthStencil(depthClean);

        return vk::RenderPassBeginInfo(
            m_primitivesPipeline, 
            );
    }

    //Change it to global Descirptor Set
    void Line::createDescriptorSetLayout(const vk::Device& device)
    {
        vk::DescriptorSetLayoutBinding uboLayoutBinding{
            0,
            vk::DescriptorType::eUniformBuffer,
            1,
            vk::ShaderStageFlagBits::eVertex};



        std::array<vk::DescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };
        vk::DescriptorSetLayoutCreateInfo layoutInfo{
            {},
            bindings
        };

        m_lineDescriptorSetLayout = device.createDescriptorSetLayout(layoutInfo);
    }

    void Line::createPrimitivePipline(const vk::Device& device, vk::Extent2D& swapChainExtent, vk::RenderPass& renderPass)
{


    auto vertShaderCode = Shader::readFile("../Assets/Shaders/line_vert.spv");
    auto fragShaderCode = Shader::readFile("../Assets/Shaders/line_frag.spv");

    vk::ShaderModule vertShaderModule = Shader::createShaderModule(device, vertShaderCode);
    vk::ShaderModule fragShaderModule = Shader::createShaderModule(device, fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
        {},
        vk::ShaderStageFlagBits::eVertex,
        vertShaderModule,
        "main"
    };

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
        {},
        vk::ShaderStageFlagBits::eFragment,
        fragShaderModule,
        "main"
    };

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages{ vertShaderStageInfo, fragShaderStageInfo };

    auto bindingDescription = getLineBindingDescription();
    auto attributeDescriptions = getLineAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        {},
        bindingDescription,
        attributeDescriptions
    );


    vk::PipelineInputAssemblyStateCreateInfo inputAssembly(vk::PipelineInputAssemblyStateCreateFlags{},
                                                           vk::PrimitiveTopology::eLineList,
                                                           VK_FALSE);


    vk::Viewport viewport(
        0.0F,
        0.0F,
        static_cast<float>(swapChainExtent.width),
        static_cast<float>(swapChainExtent.height),
        0.0F,
        1.0F);

    vk::Rect2D scissor(vk::Offset2D{ 0, 0 }, swapChainExtent);

    vk::PipelineViewportStateCreateInfo viewportState(
        vk::PipelineViewportStateCreateFlags{},
        viewport,
        scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizer(
        vk::PipelineRasterizationStateCreateFlags{},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eCounterClockwise,
        VK_FALSE,
        0.0f,
        0.0f,
        0.0f,
        2.0F);

    vk::PipelineMultisampleStateCreateInfo multisampling(
        vk::PipelineMultisampleStateCreateFlags{},
        vk::SampleCountFlagBits::e1,
        VK_FALSE);

    vk::PipelineDepthStencilStateCreateInfo depthStencil{
        {},
        true,
        true,
        vk::CompareOp::eLess,
        false,
        false,
        };

    vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        VK_FALSE,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    vk::PipelineColorBlendStateCreateInfo colorBlending(
        vk::PipelineColorBlendStateCreateFlags{},
        VK_FALSE,
        vk::LogicOp::eCopy,
        colorBlendAttachment,
        { 0.0f, 0.0f, 0.0f, 0.0f });

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(vk::PipelineLayoutCreateFlags{});


    //We don't have binding so we can crete empty pipline layout
    auto pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);


    vk::GraphicsPipelineCreateInfo pipelineInfo(
        vk::PipelineCreateFlags{},
        shaderStages,
        &vertexInputInfo,
        &inputAssembly,
        {},
        &viewportState,
        &rasterizer,
        &multisampling,
        &depthStencil,
        &colorBlending,
        {},
        pipelineLayout,
        renderPass);


        vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        VK_FALSE,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    vk::PipelineColorBlendStateCreateInfo colorBlending(
        vk::PipelineColorBlendStateCreateFlags {},
        VK_FALSE,
        vk::LogicOp::eCopy,
        colorBlendAttachment,
        { 0.0f, 0.0f, 0.0f, 0.0f });

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
        vk::PipelineLayoutCreateFlags {},
        m_lineDescriptorSetLayout);

	m_primitivePipelineCache = device.createPipelineCache(vk::PipelineCacheCreateInfo());
	auto result = device.createGraphicsPipeline(m_primitivePipelineCache, pipelineInfo);

	if (result.result != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to create primitive graphics pipline!");
	}

	m_primitivesPipeline = result.value;
}

vk::VertexInputBindingDescription Line::getLineBindingDescription()
{
    vk::VertexInputBindingDescription bindingDescription{0,sizeof(vec3),vk::VertexInputRate::eVertex};

    return bindingDescription;
}

std::array<vk::VertexInputAttributeDescription, 1> Line::getLineAttributeDescriptions()
{
    std::array<vk::VertexInputAttributeDescription, 1> attributeDescriptions{
            vk::VertexInputAttributeDescription{ //Pos
                0,
                0,
                vk::Format::eR32G32B32Sfloat,
                static_cast<uint32_t>(offsetof(LineDefinition, m_startPoint))
            }
    };

    return attributeDescriptions;
}

}
