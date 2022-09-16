#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "vulkan/vulkan.hpp"
#include <array>


namespace st::geometry
{
    struct vec2
    {
        float x;
        float y;
    };

    struct vec3
    {
        float x;
        float y;
        float z;
    };

    struct vec4
    {
        float x;
        float y;
        float z;
        float w;
    };

    struct mat4
    {
        float m_data[16] = {0};
    };


    struct Vertex
    {
        vec2 pos;
        vec3 color;

        static vk::VertexInputBindingDescription getBindingDescription();

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();
    };
}



#endif // !NAVIGATIONBAR_HPP