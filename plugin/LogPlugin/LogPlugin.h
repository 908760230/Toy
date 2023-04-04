#ifndef SPDLOG_PLUGIN_H
#define SPDLOG_PLUGIN_H

#include "IPlugin.h"
#include "IPluginManager.h"

namespace Toy
{
class LogPlugin : public IPlugin
{
public:
    LogPlugin(IPluginManager *p)
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