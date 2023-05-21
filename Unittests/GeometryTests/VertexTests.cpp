#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Geometry/Vertex.hpp>


namespace st::geometry::unittests
{


	TEST(VertexTests, VertexComparisons)
	{
		Vertex vertexFirst {
			{ 0.5f, 0.0f, 0.5f },
			{ 1.0f, 1.0f, },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};

		Vertex vertexSecond {
			{ 0.5f, 0.0f, 0.5f },
			{1.0f, 1.0f, },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f }
		};

		vertexFirst == vertexSecond;
	}


}