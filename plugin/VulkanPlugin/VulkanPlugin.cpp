#include "VulkanPlugin.h"
#include "Platform.h"
#include "VulkanRenderSystemModule.h"

using namespace Toy;

#ifdef DYNAMIC_DLL

TOY_API void DllStartPlugin(Explore::IPluginManager *pm){CREATE_PLUGIN(pm, SystemWinPlugin)};

TOY_API void DllStopPlugin(Explore::IPluginManager *pm){DESTROY_PLUGIN(pm, SystemWinPlugin)};

#endif // ! DYNAMIC_DLL

const int VulkanPlugin::GetPluginVersion()
{
    return 1;
}

std::string VulkanPlugin::GetPluginName()
{
    return GET_CLASS_NAME(VulkanPlugin);
}

void VulkanPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, RenderSystemIModule, VulkanRenderSystemModule);
}

void VulkanPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, RenderSystemIModule, VulkanRenderSystemModule)
}
