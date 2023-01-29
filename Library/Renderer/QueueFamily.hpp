#ifndef RENDERER_QUEUEFAMILY_HPP
#define RENDERER_QUEUEFAMILY_HPP

#include <optional>

namespace vk
{
    class PhysicalDevice;
    class SurfaceKHR;
}

namespace st::renderer
{

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const;




        static QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
    }; 


}
#endif // RENDERER_QUEUEFAMILY_HPP