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

}