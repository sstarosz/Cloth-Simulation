#include "SphereMesh.hpp"
#include "Math/Math.hpp"

#include <vector>
#include <numbers>

namespace st::geometry
{

	SphereMesh::SphereMesh(float radius)
	{

	}


	// Code to generate shpere
	SphereMesh::SphereMesh(float radius, uint32_t widthSubdivisions, uint32_t heightSubdivisions)
	{

		using namespace std::numbers;
		using namespace geometry;

		const int numPoints = widthSubdivisions * heightSubdivisions;
		const float widthStep = 2.0f * pi / static_cast<float>(widthSubdivisions - 1);
		const float heightStep = pi / static_cast<float>(heightSubdivisions - 1);
		std::vector<math::Vector3> points(numPoints * 3);

		for (uint32_t i = 0; i <= heightSubdivisions; i++)
		{
			const float phi = i * heightStep;
			const float sinPhi = std::sin(phi);
			const float cosPhi = std::cos(phi);

			for (uint32_t j = 0; j <= widthSubdivisions; j++)
			{
				const float theta = j * widthStep;
				const float sinTheta = std::sin(theta);
				const float cosTheta = std::cos(theta);

				float posX = radius * sinPhi * cosTheta;
				float posY = radius * sinPhi * sinTheta;
				float posZ = radius * cosPhi;

				float mappingU = static_cast<float>(j) / static_cast<float>(widthSubdivisions - 1);
				float mappingV = static_cast<float>(i) / static_cast<float>(heightSubdivisions - 1);


				float normalsX = sinPhi * cosTheta;
				float normalsY = sinPhi * sinTheta;
				float normalsZ = cosPhi;

				m_vertices.emplace_back(Vertex {
					{ posX, posY, posZ },
					{ mappingU, mappingV },
					{ 1.0f, 1.0f, 1.0f },
					{ normalsX, normalsY, normalsZ }
				});


				uint32_t index = i * (widthSubdivisions + 1) + j;
				if (i < heightSubdivisions && j < widthSubdivisions)
				{
					// Triangle 1: top-left, bottom-left, bottom-right
					m_indices.push_back(index);
					m_indices.push_back(index + widthSubdivisions + 1);
					m_indices.push_back(index + widthSubdivisions + 2);

					// Triangle 2: top-left, bottom-right, top-right
					m_indices.push_back(index);
					m_indices.push_back(index + widthSubdivisions + 2);
					m_indices.push_back(index + 1);
				}
			}
		}


	//model = Model();
	//return points;
	}

	ShapeBase::ShapeType SphereMesh::getType() const
	{
		return ShapeType::eMesh;
	}


}

