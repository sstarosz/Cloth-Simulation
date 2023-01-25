#include "ValidationLayers.hpp"



namespace st::renderer {

	template <size_t size>
	std::array<const char*, size> ValidationLayer::getValidationLayers()
	{
		const std::array validationLayers{ "VK_LAYER_KHRONOS_validation" };
		return validationLayers;
	}

}
