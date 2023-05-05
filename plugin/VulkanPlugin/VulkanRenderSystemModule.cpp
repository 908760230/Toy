#include "VulkanRenderSystemModule.h"
#include "VulkanValidationLayers.h"

using namespace Toy;

bool VulkanRenderSystemModule::Awake()
{
    m_systemWinModule = pPluginManager->FindModule<SystemWinIModule>();
    return true;
}
bool VulkanRenderSystemModule::Init()
{
    auto &validationLayers = VulkanValidationLayers::instance();
    validationLayers.enable();
    validationLayers.checkValidationLayerSupport();
    m_vulkanInstance.createInstance();
    m_vulkanInstance.createWin32Surface(m_systemWinModule->windowHandle());
    m_vulkanInstance.createLogicalDevice();
    m_vulkanInstance.createSwapChain();
    return true;
}
bool VulkanRenderSystemModule::AfterInit()
{
    return true;
}
bool VulkanRenderSystemModule::Execute()
{
    return true;
}

