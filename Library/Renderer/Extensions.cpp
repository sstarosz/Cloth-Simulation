#include "Extensions.hpp"

#include <vector>
#include "Config.hpp"
#include <vulkan/vulkan.hpp>


namespace st::renderer
{
	auto Extensions::getEnabledExtensions()
	{
		std::vector<const char*> extensions;


		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		if (true /*TODO - add debug and release mode*/)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}


		std::array extensionsResult(extensions.begin(), extensions.end());


		return extensionsResult;
	}
}


