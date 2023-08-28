#ifndef RENDERER_UNIFORMBUFFERS_HPP
#define RENDERER_UNIFORMBUFFERS_HPP

#include "Math/Math.hpp"

namespace st::renderer
{

	struct UniformBufferObject
	{
		math::Matrix4x4 model;
		math::Matrix4x4 view;
		math::Matrix4x4 proj;
	};

}

#endif //VULKANLEARNING_CONFIG_HPP_IN_HPP
