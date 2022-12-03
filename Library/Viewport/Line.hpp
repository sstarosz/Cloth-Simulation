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
        vk::Buffer getVertexBuffer() const;
        vk::DeviceSize getVertexBufferOffsets() const;

        vk::Buffer getIndexBuffer() const;


        void createDescriptorSetLayout(const vk::Device& device);


        void createPrimitivePipline(const vk::Device& device, vk::Extent2D& swapChainExtent, vk::RenderPass& renderPass);

        vk::VertexInputBindingDescription getLineBindingDescription();
        std::array<vk::VertexInputAttributeDescription, 1> getLineAttributeDescriptions();


    private:
        LineDefinition m_line;

        vk::PipelineCache m_primitivePipelineCache;
        vk::Pipeline m_primitivesPipeline;
        vk::DescriptorSetLayout m_lineDescriptorSetLayout;


    };


};

#endif // VIEWPORT_LINE_HPP
