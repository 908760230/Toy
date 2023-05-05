#ifndef VULKANVALIDATIONLAYERS_H
#define VULKANVALIDATIONLAYERS_H

#include <vulkan/vulkan.h>
#include <vector>

namespace Toy
{
class VulkanInstance;
class VulkanValidationLayers
{

public: 
    static VulkanValidationLayers &instance();
    ~VulkanValidationLayers();

    void enable()
    {
        m_enable = true;
    };
    void close()
    {
        m_enable = false;
    }
    bool isEnable() const
    {
        return m_enable;
    }

    void getValidationLayers(std::vector<const char *> &layers) const;

    VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo() const;

    void checkValidationLayerSupport() const;
    void setupDebugMessenger(VkInstance &instance);
    void destroyDebugMessenger(VkInstance &instance);

private:
    bool                      m_enable = false;
    VkDebugUtilsMessengerEXT m_debugMessenger;

private:
    VulkanValidationLayers();
};
}

#endif