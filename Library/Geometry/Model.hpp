#ifndef GEOMETRY_MODEL_HPP
#define GEOMETRY_MODEL_HPP


#include <array>
#include <memory>
#include "Vertex.hpp"

#include "Primitives/Mesh.hpp"
#include "Body/BodyBase.hpp"
#include "Primitives/ShapeBase.hpp"

namespace st::geometry
{
	struct Material
	{

	};


	class Model
	{
	public:
		Model() = default;
		Model(Mesh mesh, ShapeBase shapeBase, Material material):
			m_mesh(std::make_unique<Mesh>(mesh)),
			m_shape(std::make_unique<ShapeBase>(shapeBase)),
			m_material(std::make_unique<Material>(material))
		{ }

		std::unique_ptr<Mesh> m_mesh;
		std::unique_ptr<ShapeBase> m_shape;
		std::unique_ptr<Material> m_material;

	};


}


#endif // !GEOMETRY_VERTEX_HPP
