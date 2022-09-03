#include "QueueFamily.hpp"


#include <vector>
#include <vulkan/vulkan.hpp>


namespace st::viewport
{
	QueueFamilyIndices QueueFamilyIndices::findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
	{
        QueueFamilyIndices indices;

        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = device.getSurfaceSupportKHR(i, surface);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
	}

}

