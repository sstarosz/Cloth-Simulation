#ifndef RENDERER_EXTENSIONS_HPP
#define RENDERER_EXTENSIONS_HPP

#include <vector>

namespace st::renderer
{

	/*TODO add validationLayer enable flags*/

	class Extensions
	{
	public:
		static std::vector<const char*> getEnabledExtensions();
	};


}


#endif // !RENDERER_VALIDATION_LAYERS_HPP