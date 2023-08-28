#ifndef GEOMETRY_VERTEX_HPP
#define GEOMETRY_VERTEX_HPP

#include "vulkan/vulkan.hpp"
#include <array>
#include <ostream>
#include "Math/Math.hpp"

namespace st::geometry
{

    struct Vertex
    {
		math::Vector3 m_pos;
		math::Vector2 m_texCoord;
		math::Vector3 m_color;
		math::Vector3 m_normal;


        static vk::VertexInputBindingDescription getBindingDescription();

        static std::array<vk::VertexInputAttributeDescription, 4> getAttributeDescriptions();

        bool operator==(const Vertex&) const = default;
        auto operator<=>(const Vertex&) const = default;


        friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
		{
			os << "\nVertex(\n";
            os << "\tPos    {" << vertex.m_pos.X       << ", " << vertex.m_pos.Y      << ", " << vertex.m_pos.Z  << "}\n";
			os << "\tUV     {" << vertex.m_texCoord.X  << ", " << vertex.m_texCoord.Y << "}\n";
			os << "\tColor  {" << vertex.m_color.X     << ", " << vertex.m_color.Y    << ", " << vertex.m_color.Z  << "}\n";
			os << "\tNormal {" << vertex.m_normal.X    << ", " << vertex.m_normal.Y   << ", " << vertex.m_normal.Z << "}\n";
			os << ")\n";
			return os;
		}
    };

    using Indices = uint32_t;
}



#endif // !GEOMETRY_VERTEX_HPP
