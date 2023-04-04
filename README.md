[中文版](./doc/zh/4-插件机制.md)
# Plugin Mechanism (插件机制)
> <font size = 4> everything is a plugin! <font>

## 1. how to build and run?
   ```bash
   cd PluginMechanism
   mkdir build
   cd build
   cmake ..
   make -j4
   ```

   ```bash
    cd build/bin/Debug
    ./Main
   ```

## 2. plugin Mechanism
## 1)IModule
The class IModule is the core of plugin, it has defined many interfaces, and they are all virutal function!
```cpp
struct IModule
{
public:
    IModule() : m_bIsExecute(false), pPluginManager(nullptr)
    {
    }

    virtual ~IModule()
    {
    }

    virtual bool Awake()
    {
        return true;
    }

    virtual bool Init()
    {

        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool ReadyExecute()
    {
        return true;
    }

    virtual bool Execute()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Finalize()
    {
        return true;
    }

    virtual bool OnReloadPlugin()
    {
        return true;
    }

    virtual IPluginManager *GetPluginManager() const
    {
        return pPluginManager;
    }

    std::string name;
    bool        m_bIsExecute;

protected:
    IPluginManager *pPluginManager;
};
} // namespace Explore
```
Every single function would been executed on Corresponding stage in all programme lifetime。As shown in picture, except Execute is a loop function, they are executed only once。

![接口](doc/imgs/application.png)

## 2) Class Plugin
Actually,Class Plugin doesn't contain any transaction code. Its responsiblity is registering or unregistering derived module for Plugin Manager. The derived class of IModule has the realization of business code. For example in SpdlogPlugin：
```cpp
const int SpdlogPlugin::GetPluginVersion()
{
    return 1;
}

std::string SpdlogPlugin::GetPluginName()
{
    return GET_CLASS_NAME(SpdlogPlugin);
}

void SpdlogPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, SpdlogIModule, SpdlogModule);
}

void SpdlogPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, SpdlogIModule, SpdlogModule)
}
```
spdlogplugin is in charge of registering spdLogmodule into pluginManager, and spdlogModle has packaged the third party library-spdlog.
```cpp
#include "SpdlogModule.h"
#include "spdlog/spdlog.h"

namespace Explore
{

void SpdlogModule::Debug(const std::string &msg, const std::string &func, int line)
{
    spdlog::debug(msg, func, line);
}
void SpdlogModule::Info(const std::string &msg, const std::string &func, int line)
{
    spdlog::info(msg, func, line);
}
void SpdlogModule::Warning(const std::string &msg, const std::string &func, int line)
{
    spdlog::warn(msg, func, line);
}
void SpdlogModule::Error(const std::string &msg, const std::string &func, int line)
{
    spdlog::error(msg, func, line);
}
void SpdlogModule::Fatal(const std::string &msg, const std::string &func, int line)
{
    spdlog::critical(msg, func, line);
}
} 
```

## 3)Usage
It is very simple! In init stage,you cand find the derived module in plugin Manager through abstract class. then, call the function which you want.In this case, spdlog of Tutorial plugin print message per three seconds.
```cpp
#include "TutorialModule.h"
#include <iostream>

namespace Explore
{
bool TutorialModule::Init()
{
    std::cout << "TutorialModule Init" << std::endl;
    m_spdlog = pPluginManager->FindModule<SpdlogIModule>();
    return true;
 }
bool TutorialModule::AfterInit()
{
    m_preTimePoint = std::chrono::system_clock::now();
    return true;
 }
bool TutorialModule::Execute()
{
    PrintLogPerThreeSecond();
    return true;
 }
bool TutorialModule::BeforeShut()
{
     return true;
 }
bool TutorialModule::Shut()
{
     return true;
}
void TutorialModule::PrintLogPerThreeSecond()
{
    auto current = std::chrono::system_clock::now();
    auto diff    = current - m_preTimePoint;
    std::chrono::seconds time    = std::chrono::duration_cast<std::chrono::seconds>(diff);
    std::chrono::seconds three(3);
    if (time >= three)
    {
        m_spdlog->Info("每三秒打印日志", "PrintLogPerThreeSecond", 38);
        m_preTimePoint = current;
    }
}
}
```

## 4) Conclusion
- 1. complete orient interface programming. 
- 2. every plugin is isolate, and each plugin doesn't need conclude the realization code of derived Module class.

The code is from NoahGameFrame,An extensible plugin framework,Open source. My job is splitting the code of plugin from it.

[1]:https://github.com/ketoo/NoahGameFrame