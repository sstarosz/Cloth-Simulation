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



	Plane::Plane(Vector3 position, uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight)
	{
		if (subdivisionWidth == 0 || subdivisionHeight == 0)
		{
			throw std::runtime_error("Trying to generate Plane with 0 subdivision");
		}


		if (subdivisionWidth == 0 && subdivisionHeight == 0)
		{

			Vertex point0 
			{
				{ xPosition, xPosition, xPosition },
				{
                 uPosition, vPosition,
				 },
				{ 0.5f, 0.5f, 0.5f },
				{ 0.0f, 0.0f, 0.0f }
			};


			return;
		}

		/*1,1 - 4*/
		/*1, 2 - 6*/
		/*2, 1 - 6*/
		/*2, 2 - 8*/
		/*2, 3 -   */
		/*3, 3 - 9*/


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

		float dx = width / static_cast<float>(subdivisionWidth);
		float dz = height / static_cast<float>(subdivisionHeight);


		float startX = position.X - (width / 2.0f);
		float startZ = position.Z - (height / 2.0f);


		for (size_t i = 0; i < subdivisionWidth; i++)
		{
			for (size_t j = 0; j < subdivisionHeight; j++)
			{ 


				float xPosition { startX + (dx * j) };
				float xPosition { position.Y };
				float xPosition { startZ + (dz * i) };

				//m_geometry.push_back { }
				float uPosition = static_cast<float>(j) / static_cast<float>(subdivisionWidth - 1);
				float vPosition = static_cast<float>(i) / static_cast<float>(subdivisionHeight - 1);


				Vertex point0 {
					{ xPosition, xPosition, xPosition },
					{ uPosition, vPosition,},
					{ 0.5f, 0.5f, 0.5f },
					{ 0.0f, 0.0f, 0.0f }
				};

			}
		}
	}


}