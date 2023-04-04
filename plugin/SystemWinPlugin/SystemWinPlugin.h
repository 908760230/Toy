#ifndef TOY_SYSTEMWIN_PLUGIN_H
#define TOY_SYSTEMWIN_PLUGIN_H

#include "IPlugin.h"
#include "IPluginManager.h"

namespace Toy
{
class SystemWinPlugin : public IPlugin
{
public:
    SystemWinPlugin(IPluginManager *p)
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