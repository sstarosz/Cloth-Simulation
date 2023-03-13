#ifndef RENDERER_SURFACE_HPP
#define RENDERER_SURFACE_HPP

#include <vulkan/vulkan.hpp>


namespace st::renderer
{

	class Surface
	{
	public:
		Surface(const vk::SurfaceKHR surface);


		const vk::SurfaceKHR& getSurface() const;

	private:
		const vk::SurfaceKHR m_surface;
	};


}


#endif // !RENDERER_SURFACE_HPP