#ifndef RENDERER_INSTANCE_HPP
#define RENDERER_INSTANCE_HPP

#include "Config.hpp"
#include "DebugMessenger/DebugMessenger.hpp"
#include <vulkan/vulkan.hpp>


namespace st::renderer
{

	/*TODO Change it to ingletone?*/

	class StInstance
	{
	public:
		StInstance();

		void create();

		void destroy();


		const vk::Instance& getInstance() const;


	private:
		vk::Instance m_instance;
		DebugMessenger m_debugMessenger;
	};


}


#endif // !RENDERER_INSTANCE_HPP