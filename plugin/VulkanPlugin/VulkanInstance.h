#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include "Platform.h"
#if TOY_PLATFORM == TOY_PLATFORM_WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // TOY_PLATFORM == TOY_PLATFORM_WIN32

#include <vulkan/vulkan.h>
#include <Windows.h>
#include <vector>
#include <optional>

namespace Toy
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();
    VkInstance getVkInstance() const
    {
        return m_instance;
    }
    void createInstance();
    void createLogicalDevice();
    void createWin32Surface(HWND hwnd);
    void createSwapChain();

private:
    void getExtensions(std::vector<const char *> &result) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);



private:
    VkPhysicalDevice   pickPhysicalDevice(const VkInstance &instance);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

private:
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice         m_logicalDevice  = nullptr;
    VkQueue          m_graphicsQueue  = nullptr;
    VkQueue          m_presentQueue  = nullptr;
    VkSurfaceKHR     m_surface        = nullptr;

    VkSwapchainKHR       m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat             m_swapChainImageFormat;
    VkExtent2D           m_swapChainExtent;
};
}

#endif