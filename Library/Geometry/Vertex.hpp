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

    struct Vertex
    {
        vec3 m_pos;
		vec2 m_texCoord;
        vec3 m_color;
		vec3 m_normal;


        static vk::VertexInputBindingDescription getBindingDescription();

        static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions();
    };

    using Indices = uint32_t;
}



#endif // !GEOMETRY_VERTEX_HPP
