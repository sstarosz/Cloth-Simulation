#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Renderer/Renderer.hpp>


namespace st::renderer::unittests
{
	class RendererTests : public testing::Test
	{

	public:
		RendererTests() { }
	};


	TEST_F(RendererTests, VulkanInstanceInitialization)
	{
		//Renderer renderer;
		//renderer.initialize();
	}

}