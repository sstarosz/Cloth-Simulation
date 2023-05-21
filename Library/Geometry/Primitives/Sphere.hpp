#ifndef GEOMETRY_PRIMITIVES_SPHERE_HPP
#define GEOMETRY_PRIMITIVES_SPHERE_HPP

#include "ShapeBase.hpp"

namespace st::geometry
{

	class Sphere : public ShapeBase
	{


	public:
		Sphere(float radius);

		ShapeBase::ShapeType getType() const override;

	private:
		float m_radius;
	};


};

#endif
