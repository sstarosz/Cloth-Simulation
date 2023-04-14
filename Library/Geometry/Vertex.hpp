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

		bool operator==(const vec2&) const = default;
		auto operator<=>(const vec2&) const = default;
    };

    struct vec3
    {
        float x;
        float y;
        float z;

        bool operator==(const vec3&) const = default;
		auto operator<=>(const vec3&) const = default;
    };

    struct vec4
    {
        float x;
        float y;
        float z;
        float w;

        bool operator==(const vec4&) const = default;
		auto operator<=>(const vec4&) const = default;
    };

    struct Vertex
    {
        vec3 m_pos;
		vec2 m_texCoord;
        vec3 m_color;
		vec3 m_normal;


        static vk::VertexInputBindingDescription getBindingDescription();

        static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions();

        bool operator==(const Vertex&) const = default;
        auto operator<=>(const Vertex&) const = default;
    };

    using Indices = uint32_t;
}



#endif // !GEOMETRY_VERTEX_HPP
