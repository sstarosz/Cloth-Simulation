#ifndef RENDERER_VALIDATION_LAYERS_HPP
#define RENDERER_VALIDATION_LAYERS_HPP

#include <vulkan/vulkan.hpp>
#include <vector>

namespace st::renderer
{

	/*TODO add validationLayer enable flags*/

	class ValidationLayer
	{
	public:
		static std::vector<const char*> getValidationLayers();

		//private:
		//	constexpr static std::array m_validationLayers{ "VK_LAYER_KHRONOS_validation" };

	private:
		static bool checkValidationLayerSupport(const std::vector<const char*> validationLayers);
	};


	/*
	The layers are loaded in the order they are listed in this array,
	with the first array element being the closest to the application,
	and the last array element being the closest to the driver.
	See the Layers section for further details.
*/
}


#endif // !RENDERER_VALIDATION_LAYERS_HPP