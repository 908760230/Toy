#ifndef TOY_PLUGIN_MANAGER_H
#define TOY_PLUGIN_MANAGER_H

#include "Platform.h"
#include "IPluginManager.h"
#include <map>

namespace Toy
{
class DynLib;

class TOY_API PluginManager : public IPluginManager
{
private:
    typedef void (*DLL_START_PLUGIN_FUNC)(IPluginManager *pm);
    typedef void (*DLL_STOP_PLUGIN_FUNC)(IPluginManager *pm);

public:
    PluginManager();
    virtual ~PluginManager();

    virtual bool LoadPluginConfig() override;
    virtual bool LoadPlugin() override;

    virtual bool Awake() override;
    virtual bool Init() override;
    virtual bool AfterInit() override;
    virtual bool CheckConfig() override;
    virtual bool ReadyExecute() override;
    virtual bool Execute() override;
    virtual bool BeforeShut() override;
    virtual bool Shut() override;
    virtual bool Finalize() override;

    virtual void Registered(IPlugin *pPlugin) override;
    virtual void UnRegistered(IPlugin *pPlugin) override;

    virtual bool ReLoadPlugin(const std::string &pluginDLLName) override;

    virtual IPlugin *FindPlugin(const std::string &pluginName) override;

    virtual void AddModule(const std::string &moduleName, IModule *pModule) override;
    virtual void AddTestModule(const std::string &moduleName, IModule *pModule) override;
    virtual void RemoveModule(const std::string &moduleName) override;

    virtual IModule *FindModule(const std::string &moduleName) override;
    virtual IModule *FindTestModule(const std::string &moduleName) override;

    virtual std::list<IModule *> Modules() override;
    virtual std::list<IModule *> TestModules() override;

    virtual IPlugin *GetCurrentPlugin() override;
    virtual IModule *GetCurrentModule() override;

    virtual void SetCurrentPlugin(IPlugin *pPlugin) override;
    virtual void SetCurrentModule(IModule *pModule) override;

    virtual void        SetConfigPath(const std::string &value) override;
    virtual std::string GetConfigPath() const override;

    virtual bool IsStaticPlugin() const override;

private:
    bool CheckStaticPlugin();

    bool LoadStaticPlugin(const std::string &pluginDLLName);
    bool LoadPluginLibrary(const std::string &pluginDLLName);
    bool UnLoadPluginLibrary(const std::string &pluginDLLName);
    bool UnLoadStaticPlugin(const std::string &pluginDLLName);

    void LoadFileContent(const std::string &path, std::string &content);

private:
    uint64_t mnNowTime = 0;

    std::string mConfigPath;

    bool mIsStaticPlugin = false;

    IPlugin *mCurrentPlugin = nullptr;
    IModule *mCurrentModule = nullptr;

    std::vector<std::string>                       mStaticPlugin;
    std::map<std::string, bool>                    mPluginNameMap;
    std::map<std::string, DynLib *>                mPluginLibMap;
    std::map<std::string, IPlugin *>               mPluginInstanceMap;
    std::map<std::string, IModule *>               mModuleInstanceMap;
    std::map<std::string, IModule *>               mTestModuleInstanceMap;
    std::vector<std::pair<std::string, IModule *>> mNeedExecuteModuleVec;
};
} // namespace Toy

#endif // ! TOY_PLUGIN_MANAGER_H
