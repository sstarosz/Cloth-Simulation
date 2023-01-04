#ifndef VIEWPORT_LINE_HPP
#define VIEWPORT_LINE_HPP


#include "vulkan/vulkan.hpp"
#include <Geometry/Point3D.hpp>



namespace st::viewport
{
    struct vec3
    {
        float x;
        float y;
        float z;
    };

    struct LineDefinition
    {
        vec3 m_startPoint;
        vec3 m_stopPoint;
    };


    class Line
    {

    public:
      
        vk::Pipeline getPipeline() const;
        std::array<vk::Buffer, 1> getVertexBuffer() const;
        std::array<vk::DeviceSize, 1> getVertexBufferOffsets() const;

        vk::Buffer getIndexBuffer() const;
        uint32_t getIndexSize() const;
        vk::PipelineLayout getPiplineLayout() const;
        vk::DescriptorSet getDescritporSet(uint32_t m_currentFrame) const;


        vk::VertexInputBindingDescription getLineBindingDescription();
        std::array<vk::VertexInputAttributeDescription, 1> getLineAttributeDescriptions();

        void createPrimitivePipline(const vk::Device& device, vk::Extent2D& swapChainExtent, vk::RenderPass& renderPass);
        void createLineVertexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& graphicsQueue);
        void createLineIndexBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& graphicsQueue);
        void createDescriptorSets(const vk::Device& device, const std::vector<vk::Buffer>& uniformBuffer);

    private:
        void createBuffer(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
        void copyBuffer(const vk::Device& device, const vk::CommandPool& commandPool,
                        const vk::Queue& m_graphicsQueue,
                        vk::Buffer srcBuffer,
                        vk::Buffer dstBuffer,
            vk::DeviceSize size);
        uint32_t findMemoryType(const vk::PhysicalDevice& device, uint32_t typeFilter, vk::MemoryPropertyFlags properties);


        LineDefinition m_line;

        vk::PipelineCache m_primitivePipelineCache;
        vk::Pipeline m_primitivesPipeline;
        vk::DescriptorSetLayout m_lineDescriptorSetLayout;
        std::vector<vk::DescriptorSet> m_lineDescriptorSets;

        vk::PipelineLayout m_linePiplineLayout;
        std::vector<vk::DynamicState> m_dynamicStateEnables;
        vk::PipelineDynamicStateCreateInfo m_pipelineDynamicStateCreateInfo;

        vk::Buffer m_lineVertexBuffer;
        vk::DeviceMemory m_lineVertexBufferMemory;
        vk::Buffer m_lineIndexBuffer;
        vk::DeviceMemory m_lineIndexBufferMemory;


        const std::vector<vec3> m_lines = {
            {-2.0F,  0.0F,   0.0F,}, {2.0F, 0.0F, 0.0F,}, //x - axis 
            { 0.0F, -2.0F,   0.0F,}, {0.0F, 2.0F, 0.0F,}, //y - axis
            { 0.0F,  0.0F,  -2.0F,}, {0.0F, 0.0F, 2.0F,}, //z - axis
            };

        std::vector<uint32_t> m_linesIndices = { 0, 1, 2, 3, 4, 5};

    };


};

#endif // VIEWPORT_LINE_HPP
