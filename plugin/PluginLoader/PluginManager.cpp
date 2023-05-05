#include "PluginManager.h"
#include "DynLib.h"
#include "IPlugin.h"
#include "LogPlugin.h"
#include "Platform.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "SystemWinPlugin.h"
#include "VulkanPlugin.h"

namespace Toy
{

PluginManager::PluginManager()
{
#ifdef DYNAMIC_DLL
    mIsStaticPlugin = false;
#else
    mIsStaticPlugin = true;
#endif // DYNAMIC_DLL
}
PluginManager::~PluginManager()
{
}
bool PluginManager::LoadPluginConfig()
{
    std::string content;

    LoadFileContent(mConfigPath, content);

    return true;
}
bool PluginManager::LoadPlugin()
{
#ifndef DYNAMIC_DLL
    CREATE_PLUGIN(this, LogPlugin);
    CREATE_PLUGIN(this, SystemWinPlugin);
    CREATE_PLUGIN(this, VulkanPlugin);
#endif // !DYNAMIC_DLL        CheckStaticPlugin();

    std::cout << "----LoadPlugin----" << std::endl;
    for (auto iter = mPluginNameMap.begin(); iter != mPluginNameMap.end(); iter++)
    {
#ifdef DYNAMIC_DLL
        LoadPluginLibrary(iter->first);
#else
        LoadStaticPlugin(iter->first);
#endif // DYNAMIC_DLL
    }

#ifndef DYNAMIC_DLL
    //CheckStaticPlugin();
#endif // !DYNAMIC_DLL

    return true;
}
bool PluginManager::Awake()
{
    std::cout << "----Awake----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->Awake();
    }
    return true;
}
bool PluginManager::Init()
{
    std::cout << "----Init----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->Init();
    }
    return true;
}
bool PluginManager::AfterInit()
{
    std::cout << "----AfterInit----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->AfterInit();
    }
    return true;
}
bool PluginManager::CheckConfig()
{
    std::cout << "----CheckConfig----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->CheckConfig();
    }
    return true;
}
bool PluginManager::ReadyExecute()
{
    std::cout << "----ReadyExecute----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->ReadyExecute();
    }
    return true;
}
bool PluginManager::Execute()
{
    mnNowTime = time(NULL);

    bool bRet = true;

    for (auto &xPair : mNeedExecuteModuleVec)
    {
        bool tembRet = xPair.second->Execute();
        bRet         = bRet && tembRet;
    }

    return bRet;
}
bool PluginManager::BeforeShut()
{
    std::cout << "----BeforeShut----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->BeforeShut();
    }
    return true;
}
bool PluginManager::Shut()
{
    std::cout << "----Shut----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->Shut();
    }
    return true;
}
bool PluginManager::Finalize()
{
    std::cout << "----Finalize----" << std::endl;
    for (auto iter = mPluginInstanceMap.begin(); iter != mPluginInstanceMap.end(); iter++)
    {
        SetCurrentPlugin(iter->second);
        std::cout << iter->first << std::endl;
        iter->second->Finalize();
    }
    return true;
}
void PluginManager::Registered(IPlugin *pPlugin)
{
    const std::string pluginName = pPlugin->GetPluginName();
    if (!FindPlugin(pluginName))
    {
        mPluginInstanceMap.emplace(pluginName, pPlugin);
        pPlugin->Install();
    }
    else
    {
        std::cout << pluginName << std::endl;
        assert(0);
    }
}
void PluginManager::UnRegistered(IPlugin *pPlugin)
{
    auto it = mPluginInstanceMap.find(pPlugin->GetPluginName());
    if (it != mPluginInstanceMap.end())
    {
        it->second->Uninstall();
        delete it->second;
        it->second = NULL;
        mPluginInstanceMap.erase(it);
    }
}
bool PluginManager::ReLoadPlugin(const std::string &pluginDLLName)
{
    // 1.shut all module of this plugin
    // 2.unload this plugin
    // 3.load new plugin
    // 4.init new module
    // 5.tell others who has been reloaded
    auto itInstance = mPluginInstanceMap.find(pluginDLLName);
    if (itInstance == mPluginInstanceMap.end())
    {
        return false;
    }
    // 1
    IPlugin *pPlugin = itInstance->second;
    pPlugin->BeforeShut();
    pPlugin->Shut();
    pPlugin->Finalize();

    // 2
    auto it = mPluginLibMap.find(pluginDLLName);
    if (it != mPluginLibMap.end())
    {
        DynLib              *pLib  = it->second;
        DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

        if (pFunc)
        {
            pFunc(this);
        }

        pLib->UnLoad();

        delete pLib;
        pLib = NULL;
        mPluginLibMap.erase(it);
    }

    // 3
    DynLib *pLib  = new DynLib(pluginDLLName);
    bool    bLoad = pLib->Load();
    if (bLoad)
    {
        mPluginLibMap.emplace(pluginDLLName, pLib);

        DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
        if (!pFunc)
        {
            std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
            assert(0);
            return false;
        }

        pFunc(this);
    }
    else
    {
#if TOY_PLATFORM == TOY_PLATFORM_LINUX
        char *error = dlerror();
        if (error)
        {
            std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]"
                      << std::endl;
            std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
            assert(0);
            return false;
        }
#elif TOY_PLATFORM == TOY_PLATFORM_WIN32
        std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]"
                  << std::endl;
        std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
        assert(0);
        return false;
#endif // TOY_PLATFORM
    }

    // 4
    auto itReloadInstance = mPluginInstanceMap.begin();
    for (; itReloadInstance != mPluginInstanceMap.end(); itReloadInstance++)
    {
        if (pluginDLLName != itReloadInstance->first)
        {
            itReloadInstance->second->OnReloadPlugin();
        }
    }
    return true;
}
IPlugin *PluginManager::FindPlugin(const std::string &pluginName)
{
    auto it = mPluginInstanceMap.find(pluginName);
    if (it != mPluginInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}
void PluginManager::AddModule(const std::string &moduleName, IModule *pModule)
{
    if (!FindModule(moduleName))
    {
        mModuleInstanceMap.emplace(moduleName, pModule);

        if (pModule->m_bIsExecute)
        {
            mNeedExecuteModuleVec.push_back(std::make_pair(moduleName, pModule));
        }
    }
}
void PluginManager::AddTestModule(const std::string &moduleName, IModule *pModule)
{
    if (!FindTestModule(moduleName))
    {
        mTestModuleInstanceMap.emplace(moduleName, pModule);
    }
}
void PluginManager::RemoveModule(const std::string &moduleName)
{
    auto it = mModuleInstanceMap.find(moduleName);
    if (it != mModuleInstanceMap.end())
    {
        mModuleInstanceMap.erase(it);

        auto iter = std::find_if(mNeedExecuteModuleVec.begin(), mNeedExecuteModuleVec.end(),
                                 [&moduleName](const std::pair<std::string, IModule *> &xPair) -> bool {
                                     return xPair.first == moduleName;
                                 });

        if (iter != mNeedExecuteModuleVec.end())
            mNeedExecuteModuleVec.erase(iter);
    }
}
IModule *PluginManager::FindModule(const std::string &moduleName)
{
    if (moduleName.empty())
    {
        return NULL;
    }

    auto it = mModuleInstanceMap.find(moduleName);
    if (it != mModuleInstanceMap.end())
    {
        return it->second;
    }

    if (this->GetCurrentModule())
    {
        std::cout << this->GetCurrentModule()->name << " want to find module: " << moduleName << " but got null_ptr!!!"
                  << std::endl;
    }

    return NULL;
}
IModule *PluginManager::FindTestModule(const std::string &moduleName)
{
    if (moduleName.empty())
    {
        return NULL;
    }

    auto it = mTestModuleInstanceMap.find(moduleName);
    if (it != mTestModuleInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}
std::list<IModule *> PluginManager::Modules()
{
    std::list<IModule *> xModules;

    auto itCheckInstance = mModuleInstanceMap.begin();
    for (; itCheckInstance != mModuleInstanceMap.end(); itCheckInstance++)
    {
        xModules.push_back(itCheckInstance->second);
    }

    return xModules;
}
std::list<IModule *> PluginManager::TestModules()
{
    return std::list<IModule *>();
}
IPlugin *PluginManager::GetCurrentPlugin()
{
    return mCurrentPlugin;
}
IModule *PluginManager::GetCurrentModule()
{
    return mCurrentModule;
}
void PluginManager::SetCurrentPlugin(IPlugin *pPlugin)
{
    mCurrentPlugin = pPlugin;
}
void PluginManager::SetCurrentModule(IModule *pModule)
{
    mCurrentModule = pModule;
}
void PluginManager::SetConfigPath(const std::string &value)
{
    mConfigPath = value;
}
std::string PluginManager::GetConfigPath() const
{
    return mConfigPath;
}
bool PluginManager::IsStaticPlugin() const
{
    return mIsStaticPlugin;
}
bool PluginManager::CheckStaticPlugin()
{
    for (auto it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end();)
    {
        bool               bFind      = false;
        const std::string &pluginName = it->first;
        for (int i = 0; i < mStaticPlugin.size(); ++i)
        {
            const std::string &tempPluginName = mStaticPlugin[i];
            if (tempPluginName == pluginName)
            {
                bFind = true;
                break;
            }
        }

        if (!bFind)
        {
            it->second->Uninstall();
            delete it->second;
            it->second = NULL;

            it = mPluginInstanceMap.erase(it);
        }
        else
        {
            it++;
        }
    }

    for (auto it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end(); ++it)
    {
        std::cout << it->first << std::endl;
    }

    for (auto it = mModuleInstanceMap.begin(); it != mModuleInstanceMap.end(); ++it)
    {
        std::cout << it->first << std::endl;
    }
    return true;
}
bool PluginManager::LoadStaticPlugin(const std::string &pluginDLLName)
{
    mStaticPlugin.push_back(pluginDLLName);
    return true;
}
bool PluginManager::LoadPluginLibrary(const std::string &pluginDLLName)
{
    auto it = mPluginLibMap.find(pluginDLLName);
    if (it == mPluginLibMap.end())
    {
        DynLib *pLib  = new DynLib(pluginDLLName);
        bool    bLoad = pLib->Load();

        if (bLoad)
        {
            mPluginLibMap.emplace(pluginDLLName, pLib);

            DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
            if (!pFunc)
            {
                std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
                assert(0);
                return false;
            }

            pFunc(this);

            return true;
        }
        else
        {
#if TOY_PLATFORM == TOY_PLATFORM_LINUX || TOY_PLATFORM == TOY_PLATFORM_APPLE
            char *error = dlerror();
            if (error)
            {
                std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error
                          << "]" << std::endl;
                std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
                assert(0);
                return false;
            }
#elif TOY_PLATFORM == TOY_PLATFORM_WIN32
            std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]"
                      << std::endl;
            std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
            assert(0);
            return false;
#endif // TOY_PLATFORM
        }
    }

    return false;
}
bool PluginManager::UnLoadPluginLibrary(const std::string &pluginDLLName)
{
    auto it = mPluginLibMap.find(pluginDLLName);
    if (it != mPluginLibMap.end())
    {
        DynLib              *pLib  = it->second;
        DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

        if (pFunc)
        {
            pFunc(this);
        }

        pLib->UnLoad();

        delete pLib;
        pLib = NULL;
        mPluginLibMap.erase(it);

        return true;
    }

    return false;
}
bool PluginManager::UnLoadStaticPlugin(const std::string &pluginDLLName)
{
    return false;
}
void PluginManager::LoadFileContent(const std::string &path, std::string &content)
{
    std::ifstream fileStream;
    fileStream.open(path);
    std::stringstream ss;
    ss << fileStream.rdbuf();
    content = ss.str();
}
} // namespace Toy
