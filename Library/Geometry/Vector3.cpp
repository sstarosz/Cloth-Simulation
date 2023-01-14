#include "Vector3.hpp"

#include <cmath>

namespace st::geometry
{
	Vector3D::Vector3D()noexcept:
	x(0.0F), 
	y(0.0F),
	z(0.0F) 
	{}

	Vector3D::Vector3D(float x, float y, float z) noexcept:
	x(x),
	y(y),
	z(z) 
	{}

	//Also know as magnitude
	float Vector3D::lenght(const Vector3D& vec)
	{
		return std::sqrtf(vec.x * vec.x + 
						  vec.y * vec.y +
						  vec.y * vec.y);
	}

	//divide vector by it magnitude to make it a unit vector
	Vector3D Vector3D::normalize(const Vector3D& vec)
	{
		float norm = Vector3D::lenght(vec);
		
		if (norm > 10e-6) // TODO change to elipson
		{
			norm = 1.0F / norm;
		}
		else
		{
			norm = 0.0F;
		}

		return {vec * norm};
	}

	Vector3D Vector3D::crossProduct(const Vector3D& v, const Vector3D& w)
	{
		Vector3D u;
		u.x = v.y * w.z - v.z * w.y;
		u.y = v.z * w.x - v.x * w.z;
		u.z = v.x * w.y - v.y * w.x;
		return u;
	}
}


