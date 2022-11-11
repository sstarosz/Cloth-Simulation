#ifndef GEOMETRY_VERTEX_HPP
#define GEOMETRY_VERTEX_HPP

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
        vec3 m_pos;
        vec3 m_color;
        vec2 m_texCoord;

        static vk::VertexInputBindingDescription getBindingDescription();

        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();
    };

    using Indices = uint32_t;
}



#endif // !GEOMETRY_VERTEX_HPP
