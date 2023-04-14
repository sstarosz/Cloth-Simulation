#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Viewport/ModelsMenager.hpp>


namespace st::viewport::unittests
{
	class ModelsMenagerTests : public testing::Test
	{

	public:
		ModelsMenagerTests() { }
	};



	TEST_F(ModelsMenagerTests, VulkanWindowInitialization)
	{
		//Renderer renderer;
		//renderer.initialize();

		ModelsMenager menager;
		menager.addResource();


	}



	TEST(PrimitiveGenerator,PlaneGenerationWitdhAndHeightSubdivisionSetTo0)
	{
		Plane plane {
			Vector3 {0.0F, 0.0F, 0.0F},
			1,
			1,
			1,
			1
		};


		plane.m_geometry.at(0);
		plane.m_geometry.at(1);
		plane.m_geometry.at(1);
		plane.m_geometry.at(1);


		Vertex referenceFirstPoint {
			{ 0.5f, 0.0f, 0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};

		Vertex referenceSecondPoint {
			{ -0.5f, 0.0f, 0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};

		Vertex referenceThirdPoint {
			{ -0.5f, 0.0f, -0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};

		Vertex referenceFourthPoint {
			{ 0.5f, 0.0f, -0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};


		ASSERT_EQ(plane.m_geometry.at(0), referenceFirstPoint);
		ASSERT_EQ(plane.m_geometry.at(1), referenceSecondPoint);
		ASSERT_EQ(plane.m_geometry.at(2), referenceThirdPoint);
		ASSERT_EQ(plane.m_geometry.at(3), referenceFourthPoint);
	}

}