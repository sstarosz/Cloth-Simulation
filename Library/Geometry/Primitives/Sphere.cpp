#include "Sphere.hpp"


namespace st::geometry
{
	Sphere::Sphere(float radius): m_radius(radius) { }

	ShapeBase::ShapeType Sphere::getType() const
	{
		return ShapeType::eSphereShape;
	}

}