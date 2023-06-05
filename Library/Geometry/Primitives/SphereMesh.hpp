#ifndef GEOMETRY_PRIMITIVES_SPHEREMESH_HPP
#define GEOMETRY_PRIMITIVES_SPHEREMESH_HPP

#include "ShapeBase.hpp"

namespace st::geometry
{

	class SphereMesh : public ShapeBase
	{


	public:
		SphereMesh(float radius);
		SphereMesh(float radius, uint32_t widthSubdivisions, uint32_t heightSubdivisions);

		ShapeBase::ShapeType getType() const override;

	private:
		float m_radius;
	};


};

#endif
