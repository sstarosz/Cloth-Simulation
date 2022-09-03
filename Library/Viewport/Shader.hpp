#ifndef SHADER_HPP
#define SHADER_HPP


#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::viewport
{
    class Shader
    {

    public:
        static std::vector<char> readFile(const std::string& filename);

        static vk::ShaderModule createShaderModule(const vk::Device& device, const std::vector<char>& code);

    };


};

#endif // SWAPCHAINSUPPORT_HPP
