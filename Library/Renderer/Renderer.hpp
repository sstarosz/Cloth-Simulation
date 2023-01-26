#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>

namespace st::renderer
{

class Renderer
{
public:	
	void initialize();


	vk::Instance getInstance();

	//TODO

	//SetupDebugMessenger

	//PickUpPhysicalDevice


	//Render Pass
	//Create Graphics Pipline
	//

	
	//Initialize function


	//Clean up function

private:
	void createInstance();



	vk::Instance m_instance;
};

}


#endif // !RENDERER_RENDERER_HPP