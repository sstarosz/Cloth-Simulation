#ifndef QUEUEFAMILY_HPP
#define QUEUEFAMILY_HPP

#include <optional>

namespace vk
{
    class PhysicalDevice;
    class SurfaceKHR;
}

namespace st::viewport
{

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }



        static QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
    }; 


}
#endif // VIEWPORTWIDGET_HPP