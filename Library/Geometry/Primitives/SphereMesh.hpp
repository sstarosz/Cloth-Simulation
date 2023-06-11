#ifndef GEOMETRY_PRIMITIVES_SPHEREMESH_HPP
#define GEOMETRY_PRIMITIVES_SPHEREMESH_HPP

#include "Mesh.hpp"
#include "../Vertex.hpp"

#include <vector>

namespace st::geometry
{

	class SphereMesh : public Mesh
	{
	public:
		SphereMesh(float radius);
		SphereMesh(float radius, uint32_t widthSubdivisions, uint32_t heightSubdivisions);

		ShapeBase::ShapeType getType() const override;
	};


};

#endif
