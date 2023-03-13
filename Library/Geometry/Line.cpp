#include "Line.hpp"


namespace st::geometry
{

	vk::VertexInputBindingDescription Line::getBindingDescription()
	{
		vk::VertexInputBindingDescription bindingDescription { 0, sizeof(Line), vk::VertexInputRate::eVertex };

		return bindingDescription;
	}


	std::array<vk::VertexInputAttributeDescription, 2> Line::getAttributeDescriptions()
	{
		std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions {
			vk::VertexInputAttributeDescription { 0,
                                                  0,
												  vk::Format::eR32G32B32Sfloat,
                                                  static_cast<uint32_t>(offsetof(Line, m_pos))},
			vk::VertexInputAttributeDescription { 1,
												  0,
												  vk::Format::eR32G32B32Sfloat,
												  static_cast<uint32_t>(offsetof(Line, m_color))},
		};

		return attributeDescriptions;
	}
}
