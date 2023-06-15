#include <gmock/gmock.h>
#include <gtest/gtest.h>


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

	}


	TEST(PrimitiveGenerator, PlaneGenerationWitdhAndHeightSubdivisionSetTo1)
	{
		/*
		Plane plane {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			1,
			1
		};


		const Vertex referenceFirstPoint {
			{ -0.5f, 0.0f, 0.5f },
			{ 0.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceSecondPoint {
			{ 0.5f, 0.0f, 0.5f },
			{ 1.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceThirdPoint {
			{ -0.5f, 0.0f, -0.5f },
			{ 0.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceFourthPoint {
			{ 0.5f, 0.0f, -0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};


		ASSERT_EQ(plane.m_vertices.size(), 4);

		ASSERT_EQ(plane.m_vertices.at(0), referenceFirstPoint);
		ASSERT_EQ(plane.m_vertices.at(1), referenceSecondPoint);
		ASSERT_EQ(plane.m_vertices.at(2), referenceThirdPoint);
		ASSERT_EQ(plane.m_vertices.at(3), referenceFourthPoint);



		ASSERT_EQ(plane.m_indices.size(), 6);
		std::vector<uint32_t> indices { 0, 1, 3, 0, 3 ,2 };
		ASSERT_THAT(plane.m_indices, indices);

		*/
	}


	TEST(PrimitiveGenerator, PlaneGenerationWitdhAndHeightSubdivisionSetTo3)
	{
		/*
		Plane plane {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			3,
			3
		};


		const Vertex referenceFirstPoint {
			{ -0.5f, 0.0f, 0.5f },
			{ 0.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceSecondPoint {
			{ -0.167f, 0.0f, 0.5f },
			{ 0.333f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceThirdPoint {
			{ 0.167f, 0.0f, 0.5f },
			{ 0.667f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceFourthPoint {
			{ 0.5f, 0.0f, 0.5f },
			{ 1.0f, 0.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};



		const Vertex referenceFifthPoint {
			{ -0.5f, 0.0f, 0.167f },
			{ 0.0f, 0.333f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceSixthPoint {
			{ -0.167f, 0.0f, 0.167f },
			{ 0.333f, 0.333f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceSeventhPoint {
			{ 0.167f, 0.0f, 0.167f },
			{ 0.667f, 0.333f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceEightPoint {
			{ 0.5f, 0.0f, 0.167f },
			{ 1.0f, 0.333f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};


		const Vertex referenceNinthPoint {
			{ -0.5f, 0.0f, -0.167f },
			{  0.0, 0.667f },
			{  0.5f, 0.5f, 0.5f },
			{  0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceTenthPoint {
			{ -0.167f, 0.0f, -0.167f },
			{  0.333f, 0.667f },
			{  0.5f, 0.5f, 0.5f },
			{  0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceEleventhPoint {
			{ 0.167f, 0.0f, -0.167f },
			{ 0.667f, 0.667f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceTwelfthPoint {
			{ 0.5f, 0.0f, -0.167f },
			{ 1.0f, 0.667f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};



		const Vertex referenceThirteenthPoint {
			{  -0.5f, 0.0f, -0.5f },
			{	0.0f, 1.0f },
			{	0.5f, 0.5f, 0.5f },
			{	0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceFourteenthPoint {
			{ -0.167f, 0.0f,- 0.5f },
			{	0.333f, 1.0f },
			{	0.5f, 0.5f, 0.5f },
			{	0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceFifteenthPoint {
			{ 0.167f, 0.0f, -0.5f },
			{ 0.667f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};

		const Vertex referenceSixteenthPoint {
			{ 0.5f, 0.0f, -0.5f },
			{ 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0f }
		};



		ASSERT_EQ(plane.m_vertices.size(), 16);

		ASSERT_EQ(plane.m_vertices.at(0), referenceFirstPoint);
		ASSERT_EQ(plane.m_vertices.at(1), referenceSecondPoint);
		ASSERT_EQ(plane.m_vertices.at(2), referenceThirdPoint);
		ASSERT_EQ(plane.m_vertices.at(3), referenceFourthPoint);


		ASSERT_EQ(plane.m_vertices.at(4), referenceFifthPoint);
		ASSERT_EQ(plane.m_vertices.at(5), referenceSixthPoint);
		ASSERT_EQ(plane.m_vertices.at(6), referenceSeventhPoint);
		ASSERT_EQ(plane.m_vertices.at(7), referenceEightPoint);


		ASSERT_EQ(plane.m_vertices.at(8), referenceNinthPoint);
		ASSERT_EQ(plane.m_vertices.at(9), referenceTenthPoint);
		ASSERT_EQ(plane.m_vertices.at(10), referenceEleventhPoint);
		ASSERT_EQ(plane.m_vertices.at(11), referenceTwelfthPoint);


		ASSERT_EQ(plane.m_vertices.at(12), referenceThirteenthPoint);
		ASSERT_EQ(plane.m_vertices.at(13), referenceFourteenthPoint);
		ASSERT_EQ(plane.m_vertices.at(14), referenceFifteenthPoint);
		ASSERT_EQ(plane.m_vertices.at(15), referenceSixteenthPoint);



		ASSERT_EQ(plane.m_indices.size(), 54);
		std::vector<uint32_t> indices {
			0, 1, 5, 
			0, 5, 4,
			1, 2, 6,
			1, 6, 5,
			2, 3, 7,
			2, 7, 6,

			4, 5, 9,
			4, 9, 8,
			5, 6, 10,
			5, 10, 9,
			6, 7, 11,
			6, 11, 10,

			8, 9, 13,
			8, 13, 12,
			9, 10, 14,
			9, 14, 13,
			10, 11, 15,
			10, 15, 14,
		};
		ASSERT_THAT(plane.m_indices, indices);

		*/
	}



	TEST(PrimitiveGenerator, PlaneGenerationDifrenntWitdhAndHeightSubdivisionComparingSize)
	{
		/*
		Plane plane1x1 {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			1,
			1
		};

		Plane plane2x2 {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			2,
			2
		};

		Plane plane3x3 {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			3,
			3
		};

		Plane plane10x10 {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			10,
			10
		};

		Plane plane100x100 {
			Vector3 {0.0F, 0.0F, 0.0F},
			1.0f,
			1.0f,
			100,
			100
		};


		ASSERT_EQ(plane1x1.m_vertices.size(), 4);
		ASSERT_EQ(plane2x2.m_vertices.size(), 9);
		ASSERT_EQ(plane3x3.m_vertices.size(), 16);
		ASSERT_EQ(plane10x10.m_vertices.size(), 121);
		ASSERT_EQ(plane100x100.m_vertices.size(), 10201);

		ASSERT_EQ(plane1x1.m_indices.size(),		6);
		ASSERT_EQ(plane2x2.m_indices.size(),		24);
		ASSERT_EQ(plane3x3.m_indices.size(),		54);
		ASSERT_EQ(plane10x10.m_indices.size(),		600);
		ASSERT_EQ(plane100x100.m_indices.size(),	60000);


		*/
	}
}