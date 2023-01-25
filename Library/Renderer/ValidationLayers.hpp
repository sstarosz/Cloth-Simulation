#ifndef RENDERER_VALIDATION_LAYERS_HPP
#define RENDERER_VALIDATION_LAYERS_HPP

#include <array>

namespace st::renderer {


class ValidationLayer
{
public:
	template<size_t size>
	static std::array<const char*, size> getValidationLayers();

private:
};


}


#endif // !RENDERER_VALIDATION_LAYERS_HPP