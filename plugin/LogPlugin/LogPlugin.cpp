#include "LogPlugin.h"
#include "LogModule.h"
#include "Platform.h"

using namespace Toy;

#ifdef DYNAMIC_DLL

TOY_API void DllStartPlugin(Explore::IPluginManager *pm){CREATE_PLUGIN(pm, LogPlugin)};

TOY_API void DllStopPlugin(Explore::IPluginManager *pm){DESTROY_PLUGIN(pm, LogPlugin)};

#endif // ! DYNAMIC_DLL

const int LogPlugin::GetPluginVersion()
{
    return 1;
}

std::string LogPlugin::GetPluginName()
{
    return GET_CLASS_NAME(LogPlugin);
}

void LogPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, LogIModule, LogModule);
}

void LogPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, LogIModule, LogModule)
}
