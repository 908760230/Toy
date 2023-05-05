#ifndef VULKAN_PLUGIN_H
#define VULKAN_PLUGIN_H

#include "IPlugin.h"
#include "IPluginManager.h"

namespace Toy
{
class VulkanPlugin : public IPlugin
{
public:
    VulkanPlugin(IPluginManager *p)
    {
        pPluginManager = p;
    }
    const int GetPluginVersion() override;

    std::string GetPluginName() override;

    void Install() override;

    void Uninstall() override;
};
} // namespace Toy

#endif