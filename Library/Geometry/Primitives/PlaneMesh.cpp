#include "PlaneMesh.hpp"
#include "../Vector3.hpp"

#include <vector>
#include <numbers>

namespace st::geometry
{

	float roundFloat(const float value, uint32_t precision) noexcept
	{
		return std::round(value * std::pow(10, precision)) / std::pow(10, precision);
	}

	// Code to generate shpere
	PlaneMesh::PlaneMesh(float width, float height, uint32_t subdivisionWidth, uint32_t subdivisionHeight)
	{
		if (subdivisionWidth == 0 || subdivisionHeight == 0)
		{
			throw std::runtime_error("Trying to generate Plane with 0 subdivision");
		}

		m_vertices.reserve(static_cast<decltype(m_vertices)::size_type>((subdivisionWidth + 1) * (subdivisionHeight + 1)));
		m_indices.reserve(static_cast<decltype(m_indices)::size_type>((subdivisionWidth - 1) * (subdivisionHeight - 1) * 6));


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

		//Center
		Vector3 center { 0.0F, 0.0F, 0.0F };


		const float startX = center.X - (width / 2.0f); //-0.5
		const float startZ = center.Z + (height / 2.0f); //-0.5


		const float texDx = 1.0f / static_cast<float>(subdivisionWidth);
		const float texDy = 1.0f / static_cast<float>(subdivisionHeight);


		const float uStartPosition { 0.0f };
		const float vStartPosition { 0.0f };

		for (uint32_t i = 0; i <= subdivisionWidth; i++)
		{
			for (uint32_t j = 0; j <= subdivisionHeight; j++)
			{

				const float positionX = roundFloat((startX + (dx * j)), precision);
				const float positionY = roundFloat(center.Y, precision);
				const float positionZ = roundFloat((startZ - (dz * i)), precision);

				const float texPositionU = roundFloat((uStartPosition + texDx * j), precision);
				const float texPositionV = roundFloat((vStartPosition + texDy * i), precision);


				m_vertices.emplace_back(Vertex {
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
	}

	ShapeBase::ShapeType PlaneMesh::getType() const
	{
		return ShapeType::eMesh;
	}


}
