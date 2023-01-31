#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"

#include <optional>

namespace st::renderer
{

class Renderer
{
public:	

	Renderer(const StInstance& instance, const Surface& surface);

	void initialize();
	void releaseResources();


	vk::Instance getInstance() const;

	//TODO - Delete after refactor
	vk::SurfaceKHR getSurface() const;
	vk::PhysicalDevice getPhysicalDevice() const;





private:


	const StInstance& m_instance;
	const Surface& m_surface;
	PhysicalDevice m_physicalDevice;
};

}


#endif // !RENDERER_RENDERER_HPP