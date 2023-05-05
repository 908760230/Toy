#ifndef VULKANPHYSICDEVICEMANAGER_H
#define VULKANPHYSICDEVICEMANAGER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace Toy
{
    
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

}

#endif