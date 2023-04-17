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



	float roundFloat(const float value, uint32_t precision)  noexcept
	{ 
		return std::round(value * std::pow(10, precision)) / std::pow(10, precision);
	}

	Plane::Plane(Vector3 center, float width, float height, uint32_t subdivisionWidth, uint32_t subdivisionHeight)
	{
		if (subdivisionWidth == 0 || subdivisionHeight == 0)
		{
			throw std::runtime_error("Trying to generate Plane with 0 subdivision");
		}

		m_geometry.reserve(static_cast<decltype(m_geometry)::size_type>((subdivisionWidth + 1) * (subdivisionHeight + 1)));
		m_indices.reserve(static_cast<decltype(m_geometry)::size_type>((subdivisionWidth - 1) * (subdivisionHeight - 1) * 6));


		/*UV - (0,0) - (1,1)*/
		/*Vertex Position 
		x - right left
		y - down up
		z - back forward
		*/
		
		/*Points are generated*/


		/* width 1*/
		/* hight 1*/
		
		/* width step 1 (1) -> 0.5 (2) -> 0.3 (3) -> 0.25(4)*/
		/* width step 1 (1) -> 0.5 (2) -> 0.3 (3) -> 0.25(4)*/

		/* start position -0.5, -0.5*/

		const size_t precision = 3.0;

		const float dx = width / static_cast<float>(subdivisionWidth);
		const float dz = height / static_cast<float>(subdivisionHeight);


		const float startX = center.X - (width / 2.0f);	   //-0.5
		const float startZ = center.Z + (height / 2.0f);   //-0.5

		const float uStartPosition{0.0f};
		const float vStartPosition{0.0f};

		for (uint32_t i = 0; i <= subdivisionWidth; i++)
		{
			for (uint32_t j = 0; j <= subdivisionHeight; j++)
			{ 

				const float positionX = roundFloat((startX + (dx * j)), precision);
				const float positionY = roundFloat(center.Y, precision);
				const float positionZ = roundFloat((startZ - (dz * i)), precision);

				const float texPositionU = roundFloat((uStartPosition + dx * j), precision);
				const float texPositionV = roundFloat((vStartPosition + dz * i), precision);


				m_geometry.emplace_back(Vertex{
					{ positionX, positionY, positionZ },
					{ texPositionU, texPositionV },
					{ 0.5f, 0.5f, 0.5f },
					{ 0.0f, 1.0f, 0.0f }
                });


				if (i < subdivisionWidth && j < subdivisionHeight)
				{
					const uint32_t baseIndex = j + i * (subdivisionWidth + 1);
					m_indices.push_back(baseIndex);
					m_indices.push_back(baseIndex + 1);
					m_indices.push_back(baseIndex + subdivisionWidth + 2);
					m_indices.push_back(baseIndex);
					m_indices.push_back(baseIndex + subdivisionWidth + 2);
					m_indices.push_back(baseIndex + subdivisionWidth + 1);
				}
			}
		}



		//generate indices
	}


}