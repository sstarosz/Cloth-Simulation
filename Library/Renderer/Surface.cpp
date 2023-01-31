#include "Surface.hpp"



namespace st::renderer 
{

	Surface::Surface(const vk::SurfaceKHR surface):
	m_surface(surface)
	{ 

	}


	const vk::SurfaceKHR& Surface::getSurface() const
	{
		return m_surface;
	}

	

}
	
	
