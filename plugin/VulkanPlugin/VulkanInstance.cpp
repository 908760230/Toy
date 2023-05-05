#include "VulkanInstance.h"
#include "VulkanValidationLayers.h"
#include "VulkanPhysicDeviceManager.h"
#include <algorithm>
#include <stdexcept>
#include <set>
#include <limits>
#include "Platform.h"

using namespace Toy;;

VulkanInstance::VulkanInstance()
{
}

VulkanInstance::~VulkanInstance()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    VulkanValidationLayers::instance().destroyDebugMessenger(m_instance);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanInstance::createWin32Surface(HWND hwnd)
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd                        = hwnd;
    createInfo.hinstance                   = GetModuleHandle(nullptr);

    auto CreateWin32SurfaceKHR =
        (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(m_instance, "vkCreateWin32SurfaceKHR");
    if (!CreateWin32SurfaceKHR ||
        CreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("fail to create win32 surface");
    }
}

void VulkanInstance::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
    VkSurfaceFormatKHR      surfaceFormat    = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR        presentMode      = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D              extent           = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices              = findQueueFamilies(m_physicalDevice);
    uint32_t           queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent      = extent;
}

void VulkanInstance::getExtensions(std::vector<const char *> &result) const
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    for (size_t index = 0; index < extensionCount; index++)
    {
        if (strcmp(extensions[index].extensionName, "VK_KHR_device_group_creation") == 0)
            result.emplace_back("VK_KHR_device_group_creation");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_display") == 0)
            result.emplace_back("VK_KHR_display");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_external_fence_capabilities") == 0)
            result.emplace_back("VK_KHR_external_fence_capabilities");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_external_memory_capabilities") == 0)
            result.emplace_back("VK_KHR_external_memory_capabilities");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_external_semaphore_capabilities") == 0)
            result.emplace_back("VK_KHR_external_semaphore_capabilities");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_get_display_properties2") == 0)
            result.emplace_back("VK_KHR_get_display_properties2");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_get_physical_device_properties2") == 0)
            result.emplace_back("VK_KHR_get_physical_device_properties2");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_get_surface_capabilities2") == 0)
            result.emplace_back("VK_KHR_get_surface_capabilities2");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_surface") == 0)
            result.emplace_back("VK_KHR_surface");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_win32_surface") == 0)
            result.emplace_back("VK_KHR_win32_surface");
        else if (strcmp(extensions[index].extensionName, "VK_EXT_debug_report") == 0)
            result.emplace_back("VK_EXT_debug_report");
        else if (strcmp(extensions[index].extensionName, "VK_EXT_debug_utils") == 0)
            result.emplace_back("VK_EXT_debug_utils");
        else if (strcmp(extensions[index].extensionName, "VK_EXT_direct_mode_display") == 0)
            result.emplace_back("VK_EXT_direct_mode_display");
        else if (strcmp(extensions[index].extensionName, "VK_EXT_swapchain_colorspace") == 0)
            result.emplace_back("VK_EXT_swapchain_colorspace");
        else if (strcmp(extensions[index].extensionName, "VK_NV_external_memory_capabilities") == 0)
            result.emplace_back("VK_NV_external_memory_capabilities");
        else if (strcmp(extensions[index].extensionName, "VK_KHR_portability_enumeration") == 0)
            result.emplace_back("VK_KHR_portability_enumeration");

    }

    auto validationLayers = VulkanValidationLayers::instance();
    if (validationLayers.isEnable())
        result.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

bool VulkanInstance::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanInstance::createInstance()
{
    VkApplicationInfo appInfo  = {};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "Toy";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "Toy Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    std::vector<const char *> extensions;
    getExtensions(extensions);

    VkInstanceCreateInfo createInfo                    = {};
    createInfo.sType                                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo                        = &appInfo;
    createInfo.enabledExtensionCount                   = extensions.size();
    createInfo.ppEnabledExtensionNames                 = extensions.data();
    auto                      validationLayersInstance = VulkanValidationLayers::instance();
    std::vector<const char *> validationLayers;
    if (validationLayersInstance.isEnable())
    {
        validationLayersInstance.getValidationLayers(validationLayers);
        createInfo.enabledLayerCount   = validationLayers.size();
        createInfo.ppEnabledLayerNames = validationLayers.data();
        auto debugMessagerCreateInfo   = validationLayersInstance.populateDebugMessengerCreateInfo();
        createInfo.pNext               = &debugMessagerCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext             = nullptr;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS)
        throw std::runtime_error("failed to create vkInstace!");

    validationLayersInstance.setupDebugMessenger(m_instance);

}

void VulkanInstance::createLogicalDevice()
{
    m_physicalDevice = pickPhysicalDevice(m_instance);

    QueueFamilyIndices                   indices = findQueueFamilies(m_physicalDevice);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority              = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }
    

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    auto                      validationLayersInstance = VulkanValidationLayers::instance();
    std::vector<const char *> validationLayers;
    if (validationLayersInstance.isEnable())
    {
        validationLayersInstance.getValidationLayers(validationLayers);
        createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
}

VkPhysicalDevice VulkanInstance::pickPhysicalDevice(const VkInstance &instance)
{
    VkPhysicalDevice physicalDevice = nullptr;
    uint32_t         deviceCount    = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
    return physicalDevice;
}

QueueFamilyIndices VulkanInstance::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int index = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = index;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = index;
        }

        if (indices.isComplete())
            break;
        index++;
    }
    return indices;
}

bool VulkanInstance::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails VulkanInstance::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details = {};
    auto result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
    
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR VulkanInstance::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanInstance::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanInstance::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width = 800, height = 600;

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

