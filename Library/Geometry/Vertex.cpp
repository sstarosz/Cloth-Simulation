#include "Vertex.hpp"


namespace st::geometry
{

    vk::VertexInputBindingDescription Vertex::getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription{
            0,
            sizeof(Vertex),
            vk::VertexInputRate::eVertex
        };

        return bindingDescription;
    }


    std::array<vk::VertexInputAttributeDescription, 4> Vertex::getAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 4> attributeDescriptions{
            vk::VertexInputAttributeDescription{
                0,
                0,
                vk::Format::eR32G32B32Sfloat,
                static_cast<uint32_t>(offsetof(Vertex, m_pos))
            },
            vk::VertexInputAttributeDescription{
                1,
                0,
                vk::Format::eR32G32Sfloat,
                static_cast<uint32_t>(offsetof(Vertex, m_texCoord))
            },
            vk::VertexInputAttributeDescription{
                2,
                0,
                vk::Format::eR32G32B32Sfloat,
                static_cast<uint32_t>(offsetof(Vertex, m_color))
            },
			vk::VertexInputAttributeDescription {
			    3,
                0,
                vk::Format::eR32G32B32Sfloat,
			    static_cast<uint32_t>(offsetof(Vertex, m_normal))
            }
        };

        return attributeDescriptions;
    }
}
