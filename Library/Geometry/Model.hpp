#ifndef GEOMETRY_MODEL_HPP
#define GEOMETRY_MODEL_HPP


#include <array>
#include <memory>
#include "Vertex.hpp"

#include "Body/BodyBase.hpp"
#include "Primitives/ShapeBase.hpp"
#include "Body/BodyBase.hpp"

namespace st::geometry
{
	struct Texture
	{


		//TODO clean buffer at the end
		uint32_t textureWidth;
		uint32_t textureHeight;
		uint32_t texChannels;

		std::span<std::byte> pixels;
	};


	struct Material
	{
		Texture m_texture;
	};





	class Model
	{
	public:
		Model(std::unique_ptr<ShapeBase>&& shapeBase,
			  std::unique_ptr<BodyBase>&& bodyBase,
			  std::unique_ptr<Material>&& material):
			m_shape(std::move(shapeBase)),
			m_bodyBase(std::move(bodyBase)),
			m_material(std::move(material))
		{ }

		std::unique_ptr<ShapeBase> m_shape;
		std::unique_ptr<BodyBase> m_bodyBase;
		std::unique_ptr<Material> m_material;

	};


}


#endif // !GEOMETRY_VERTEX_HPP
