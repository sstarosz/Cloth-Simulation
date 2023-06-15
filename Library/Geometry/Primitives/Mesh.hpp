#ifndef GEOMETRY_PRIMITIVES_MESH_HPP
#define GEOMETRY_PRIMITIVES_MESH_HPP

#include "ShapeBase.hpp"
#include "../Vertex.hpp"

#include <vector>

namespace st::geometry
{

	class Mesh : public ShapeBase
	{


	public:
		Mesh();

		ShapeBase::ShapeType getType() const override;

	//protected:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};


};

#endif
