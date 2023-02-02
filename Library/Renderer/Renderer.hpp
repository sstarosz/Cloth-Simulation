#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include "DebugMessenger/DebugMessenger.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"

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
	const vk::SurfaceKHR& getSurface() const;
	const vk::PhysicalDevice& getPhysicalDevice() const;
	const vk::Device& getLogicalDevice() const;
	const vk::Queue& getGraphicsQueue() const;
	const vk::Queue& getPresentationQueue() const;




private:


	const StInstance& m_instance;
	const Surface& m_surface;
	PhysicalDevice m_physicalDevice;
	LogicalDevice m_logicalDevice;
};

}


#endif // !RENDERER_RENDERER_HPP