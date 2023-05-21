#ifndef RENDERER_UNIFORMBUFFERS_HPP
#define RENDERER_UNIFORMBUFFERS_HPP

#include <Geometry/Matrix4x4.hpp>

namespace st::renderer
{

	struct UniformBufferObject
	{
		geometry::Matrix4x4 model;
		geometry::Matrix4x4 view;
		geometry::Matrix4x4 proj;
	};

}

#endif //VULKANLEARNING_CONFIG_HPP_IN_HPP
