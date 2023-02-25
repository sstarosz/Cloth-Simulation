#ifndef GEOMETRY_LINE_HPP
#define GEOMETRY_LINE_HPP

#include "vulkan/vulkan.hpp"
#include <array>
#include "Vertex.hpp"

namespace st::geometry
{



	struct Line
	{
		vec3 m_pos;
		vec3 m_color;


		static vk::VertexInputBindingDescription getBindingDescription();

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();
	};


}


#endif // !GEOMETRY_VERTEX_HPP
