#ifndef GEOMETRY_PRIMITIVES_PLANEMESH_HPP
#define GEOMETRY_PRIMITIVES_PLANEMESH_HPP

#include "Mesh.hpp"
#include "../Vertex.hpp"

#include <vector>

namespace st::geometry
{

	class PlaneMesh : public Mesh
	{
	public:
		PlaneMesh(float radius);
		PlaneMesh(float width, float height, uint32_t subdivisionWidth, uint32_t subdivisionHeight);

		ShapeBase::ShapeType getType() const override;
	};


};

#endif
