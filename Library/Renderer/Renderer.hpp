#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"
#include "Surface.hpp"

#include <optional>

namespace st::renderer
{

class Renderer
{
public:	

	Renderer();


	void setupSurface(const vk::SurfaceKHR& surface);

	void createInstance();
	void initialize();
	void releaseResources();


	vk::Instance getInstance() const;
	vk::SurfaceKHR getSurface() const;

	//TODO

	//SetupDebugMessenger

	//PickUpPhysicalDevice


	//Render Pass
	//Create Graphics Pipline
	//

	
	//Initialize function


	//Clean up function

private:


	vk::Instance m_instance;
	DebugMessenger m_debugMessenger;
	std::optional<Surface> m_surface;

};

}


#endif // !RENDERER_RENDERER_HPP