#include "Vector3.hpp"

#include <cmath>

namespace st::geometry
{
	//Also know as magnitude
	float Vector3::lenght(const Vector3& Vec) //TODO make it constexpr
	{
		return std::sqrt(Vec.X * Vec.X + 
						  Vec.Y * Vec.Y +
						  Vec.Z * Vec.Z);
	}

	//divide vector by it magnitude to make it a unit vector
	Vector3 Vector3::normalize(const Vector3& Vec) //TODO make it constexpr
	{
		float norm = Vector3::lenght(Vec);
		
		if (norm > 10e-6) // TODO change to elipson
		{
			norm = 1.0F / norm;
		}
		else
		{
			norm = 0.0F;
		}

		return { Vec * norm };
	}

}


