#include "Application.h"

namespace Toy
{
void Application::Init()
{
    mPluginManager = std::make_shared<PluginManager>();
    mPluginManager->LoadPluginConfig();
    mPluginManager->LoadPlugin();

    mPluginManager->Awake();
    mPluginManager->Init();
    mPluginManager->AfterInit();
    mPluginManager->CheckConfig();
    mPluginManager->ReadyExecute();
}

void Application::Excute()
{
    mPluginManager->Execute();
}

void Application::Shutdown()
{
    mPluginManager->BeforeShut();
    mPluginManager->Shut();
    mPluginManager->Finalize();
}

} // namespace Toy
