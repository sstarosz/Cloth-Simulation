#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"

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

	//TODO - Delete after refactor
	vk::SurfaceKHR getSurface() const;
	vk::PhysicalDevice getPhysicalDevice() const;





private:


	vk::Instance m_instance;
	DebugMessenger m_debugMessenger;
	std::optional<Surface> m_surface;
	PhysicalDevice m_physicalDevice;
};

}


#endif // !RENDERER_RENDERER_HPP