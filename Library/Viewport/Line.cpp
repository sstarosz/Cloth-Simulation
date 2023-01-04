#include "Line.hpp"
#include "Shader.hpp"

namespace st::viewport
{



    vk::Pipeline Line::getPipeline() const
    {
        return m_primitivesPipeline;
    }

    std::array<vk::Buffer, 1> Line::getVertexBuffer() const
    {
        return std::array<vk::Buffer, 1>{m_lineVertexBuffer};
    }

    std::array<vk::DeviceSize, 1> Line::getVertexBufferOffsets() const
    {
        return std::array<vk::DeviceSize, 1>{0};
    }

    vk::Buffer Line::getIndexBuffer() const
    {
        return m_lineIndexBuffer;
    }

    uint32_t Line::getIndexSize() const
    {
        return m_linesIndices.size();
    }

    vk::PipelineLayout Line::getPiplineLayout() const
    {
        return m_linePiplineLayout;
    }

    vk::DescriptorSet Line::getDescritporSet(uint32_t m_currentFrame) const
    {
        return m_lineDescriptorSets[m_currentFrame];
    }

    vk::VertexInputBindingDescription Line::getLineBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription{
            0, //binding index
            sizeof(vec3),
            vk::VertexInputRate::eVertex
        };

        return bindingDescription;
    }

    std::array<vk::VertexInputAttributeDescription, 1> Line::getLineAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 1> attributeDescriptions{
                vk::VertexInputAttributeDescription{ //Pos
                    0, //location
                    0, //binding
                    vk::Format::eR32G32B32Sfloat,
                    0 //Only position is used so offset 0
                }
        };

        return attributeDescriptions;
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


   //vk::Viewport viewport(
   //    0.0F,
   //    0.0F,
   //    static_cast<float>(swapChainExtent.width),
   //    static_cast<float>(swapChainExtent.height),
   //    0.0F,
   //    1.0F);
   //
   //vk::Rect2D scissor(vk::Offset2D{ 0, 0 }, swapChainExtent);

    vk::PipelineViewportStateCreateInfo viewportState(
        vk::PipelineViewportStateCreateFlags{} //,
        //viewport,
        //scissor
        );

    vk::PipelineRasterizationStateCreateInfo rasterizer(
        vk::PipelineRasterizationStateCreateFlags{},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eNone,
        vk::FrontFace::eCounterClockwise,
        VK_FALSE,
        0.0f,
        0.0f,
        0.0f,
        1.0F);

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

    m_dynamicStateEnables = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    m_pipelineDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{ {}, m_dynamicStateEnables };

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


    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
        vk::PipelineLayoutCreateFlags{},
        m_lineDescriptorSetLayout
        );

    //We don't have binding so we can crete empty pipline layout
    m_linePiplineLayout = device.createPipelineLayout(pipelineLayoutInfo);


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
        &m_pipelineDynamicStateCreateInfo,
        m_linePiplineLayout,
        renderPass);



	m_primitivePipelineCache = device.createPipelineCache(vk::PipelineCacheCreateInfo());
	auto result = device.createGraphicsPipeline(m_primitivePipelineCache, pipelineInfo);

	if (result.result != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to create primitive graphics pipline!");
	}

	m_primitivesPipeline = result.value;
}

void Line::createLineVertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& graphicsQueue)
{
    vk::DeviceSize bufferSize = sizeof(m_lines[0]) * m_lines.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;


    createBuffer(physicalDevice,
        device,
        bufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer,
        stagingBufferMemory
    );

    void* data = device.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, m_lines.data(), (size_t)bufferSize); //vertices should fullfil trival object specyfication?
    device.unmapMemory(stagingBufferMemory);

    createBuffer(
        physicalDevice,
        device,
        bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_lineVertexBuffer,
        m_lineVertexBufferMemory);

    copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, m_lineVertexBuffer, bufferSize);
    device.destroyBuffer(stagingBuffer);
    device.freeMemory(stagingBufferMemory);
}

void Line::createLineIndexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& graphicsQueue)
{
    vk::DeviceSize bufferSize = sizeof(m_linesIndices[0]) * m_linesIndices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    createBuffer(
        physicalDevice,
        device,
        bufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        stagingBuffer,
        stagingBufferMemory);

    void* data = device.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, m_linesIndices.data(), (size_t)bufferSize);
    device.unmapMemory(stagingBufferMemory);

    createBuffer(
        physicalDevice,
        device,
        bufferSize,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal,
        m_lineIndexBuffer,
        m_lineIndexBufferMemory);

    copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, m_lineIndexBuffer, bufferSize);

    device.destroyBuffer(stagingBuffer);
    device.freeMemory(stagingBufferMemory);
}

void Line::createDescriptorSets(const vk::Device& device, const std::vector<vk::Buffer>& uniformBuffer)
{
    //Create Descriptor Pool
    vk::DescriptorPoolSize poolSize{ vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(2) };

    vk::DescriptorPoolCreateInfo poolInfo{ {}, static_cast<uint32_t>(2), poolSize };

    auto lineDescriptorPool = device.createDescriptorPool(poolInfo);


    //Create Descriptor Set Layout
    vk::DescriptorSetLayoutBinding uboLayoutBinding{
    0,
    vk::DescriptorType::eUniformBuffer,
    1,
    vk::ShaderStageFlagBits::eVertex
    };

    std::array<vk::DescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };
    vk::DescriptorSetLayoutCreateInfo layoutInfo{
        {},
        bindings
    };

    m_lineDescriptorSetLayout = device.createDescriptorSetLayout(layoutInfo);


    //Create Descriptor Sets
    std::vector<vk::DescriptorSetLayout> layouts(2, m_lineDescriptorSetLayout);
    vk::DescriptorSetAllocateInfo allocInfo{
        lineDescriptorPool,
        layouts
    };

    m_lineDescriptorSets = device.allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < 2; ++i) {
        vk::DescriptorBufferInfo bufferInfo{
            uniformBuffer.at(i),
            0,
            192 //TODO to Fix
        };
        std::array<vk::WriteDescriptorSet, 1> descriptorWrites{
            vk::WriteDescriptorSet {
                m_lineDescriptorSets.at(i),
                0,
                0,
                vk::DescriptorType::eUniformBuffer,
                {},
                bufferInfo,
                {} }
        };

        device.updateDescriptorSets(descriptorWrites, {});
    }
}


void Line::createBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
{
    vk::BufferCreateInfo bufferInfo{ {}, size, usage, vk::SharingMode::eExclusive };

    buffer = device.createBuffer(bufferInfo);

    vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo{ memoryRequirements.size,
        findMemoryType(physicalDevice,memoryRequirements.memoryTypeBits, properties) };

    bufferMemory = device.allocateMemory(allocInfo);

    device.bindBufferMemory(buffer, bufferMemory, 0);
}

void Line::copyBuffer(const vk::Device& device,
                      const vk::CommandPool& commandPool,
                      const vk::Queue& graphicsQueue,
                      vk::Buffer srcBuffer,
                      vk::Buffer dstBuffer,
                      vk::DeviceSize size)
{
    vk::CommandBufferAllocateInfo allocInfo{ commandPool, vk::CommandBufferLevel::ePrimary, 1 };

    auto commandBuffer = device.allocateCommandBuffers(allocInfo);

    vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

    commandBuffer.at(0).begin(beginInfo);

    vk::BufferCopy copyRegin{ 0, 0, size };

    commandBuffer.at(0).copyBuffer(srcBuffer, dstBuffer, 1, &copyRegin);

    commandBuffer.at(0).end();

    vk::SubmitInfo submitInfo{ {}, {}, commandBuffer };

    graphicsQueue.submit(1, &submitInfo, {}); //TODO GraphicQueue should copy buffers?
    graphicsQueue.waitIdle();

    device.freeCommandBuffers(commandPool, commandBuffer);
}

uint32_t Line::findMemoryType(const vk::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

}
