#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"

namespace st::renderer
{

class Renderer
{
public:	

	Renderer();

	void initialize();
	void releaseResources();


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
	DebugMessenger m_debugMessenger;

};

}


#endif // !RENDERER_RENDERER_HPP