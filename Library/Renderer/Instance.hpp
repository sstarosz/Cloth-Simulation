#ifndef RENDERER_INSTANCE_HPP
#define RENDERER_INSTANCE_HPP

#include "Config.hpp"
#include <vulkan/vulkan.hpp>

namespace st::renderer {

/*TODO Change it to ingletone?*/

class Instance
{


public:
	vk::Instance getInstance();



private:
	vk::Instance m_instance;
};


}


#endif // !RENDERER_INSTANCE_HPP