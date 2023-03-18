#include "ModelsMenager.hpp"

#include <vector>
#include <numbers>
#include <cmath>

namespace st::viewport
{

	ModelsMenager::ModelsMenager()
	{

	}

	void ModelsMenager::addModel(Model& model)
	{ 
		m_models.emplace_back(std::make_unique<Model>(model));
	}


	Sphere::Sphere(geometry::Vector3 position, float radius, uint32_t widthSubdivisions, uint32_t heightSubdivisions): Model {}
	{
		using namespace std::numbers;
		using namespace geometry;

		const int numPoints = widthSubdivisions * heightSubdivisions;
		const float widthStep = 2.0f * pi / static_cast<float>(widthSubdivisions - 1);
		const float heightStep = pi / static_cast<float>(heightSubdivisions - 1);
		std::vector<Vector3> points(numPoints * 3);

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

				float posX = radius * sinPhi * cosTheta + position.X;
				float posY = radius * sinPhi * sinTheta + position.Y;
				float posZ = radius * cosPhi + position.Z;

				float mappingU = static_cast<float>(j) / static_cast<float>(widthSubdivisions - 1);
				float mappingV = static_cast<float>(i) / static_cast<float>(heightSubdivisions - 1);


				float normalsX = sinPhi * cosTheta;
				float normalsY = sinPhi * sinTheta;
				float normalsZ = cosPhi;

				m_geometry.emplace_back(Vertex {
					{ posX, posY, posZ },
					{ mappingU, mappingV },
					{ 1.0f, 1.0f, 1.0f },
					{ normalsX, normalsY, normalsZ }
                });
			}
		}





		 //model = Model();
		//return points;
	}
}