#include "ModelsMenager.hpp"

#include <vector>
#include <numbers>
#include <cmath>

namespace st::viewport
{
	ModelsMenager::ModelsMenager()
	{

	}


	Sphere::Sphere(geometry::Vector3 position, float radius, uint32_t widthSubdivisions, uint32_t heightSubdivisions)
	{
		using namespace std::numbers;


		const int numPoints = widthSubdivisions * heightSubdivisions;
		const float widthStep = 2.0f * pi / static_cast<float>(widthSubdivisions - 1);
		const float heightStep = pi / static_cast<float>(heightSubdivisions - 1);
		std::vector<float> points(numPoints * 3);

		 for (int i = 0; i < heightSubdivisions; i++)
		{
			const float phi = i * heightStep;
			const float sinPhi = std::sin(phi);
			const float cosPhi = std::cos(phi);

			for (int j = 0; j < widthSubdivisions; j++)
			{
				const float theta = j * widthStep;
				const float sinTheta = std::sin(theta);
				const float cosTheta = std::cos(theta);

				const int index = (i * widthSubdivisions + j) * 3;
				points[index] = radius * sinPhi * cosTheta + position.X;
				points[index + 1] = radius * sinPhi * sinTheta + position.Y;
				points[index + 2] = radius * cosPhi + position.Z;
			}
		}



		//return points;
	}
}