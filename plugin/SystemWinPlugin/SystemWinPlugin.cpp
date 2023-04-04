#include "SystemWinPlugin.h"
#include "Platform.h"
#include "SystemIModule.h"
#include "SystemWinModule.h"

using namespace Toy;

#ifdef DYNAMIC_DLL

TOY_API void DllStartPlugin(Explore::IPluginManager *pm){CREATE_PLUGIN(pm, SystemWinPlugin)};

TOY_API void DllStopPlugin(Explore::IPluginManager *pm){DESTROY_PLUGIN(pm, SystemWinPlugin)};

#endif // ! DYNAMIC_DLL

const int SystemWinPlugin::GetPluginVersion()
{
    return 1;
}

std::string SystemWinPlugin::GetPluginName()
{
    return GET_CLASS_NAME(SystemWinPlugin);
}

void SystemWinPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, SystemIModule, SystemWinModule);
}

void SystemWinPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, SystemIModule, SystemWinModule)
}
