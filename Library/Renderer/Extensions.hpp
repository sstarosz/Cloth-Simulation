#ifndef RENDERER_EXTENSIONS_HPP
#define RENDERER_EXTENSIONS_HPP

#include <array>

namespace st::renderer {


class Extensions
{
public:
	static auto getEnabledExtensions();

	private:
		constexpr static std::array m_validationLayers
			= { "VK_LAYER_KHRONOS_validation" };
};


}


#endif // !RENDERER_VALIDATION_LAYERS_HPP