#include "ValidationLayers.hpp"


namespace st::renderer {

	std::vector<const char*> ValidationLayer::getValidationLayers()
	{


		std::vector<const char*> validationLayers { "VK_LAYER_KHRONOS_validation" };

		if (checkValidationLayerSupport(validationLayers))
		{
			//TODO - Better validation layer handling
			throw std::runtime_error("validation layer requested, but not available!");
		}


		return validationLayers;
	}


	bool ValidationLayer::checkValidationLayerSupport(const std::vector<const char*> validationLayers)
	{
		std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

		for (const auto* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

}
