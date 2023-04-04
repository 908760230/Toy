#ifndef TOY_IPLUGINMANAGER_H
#define TOY_IPLUGINMANAGER_H

#include <cassert>
#include <functional>
#include <list>
#include <string>
#include <vector>

namespace Toy
{
class IPlugin;
class IModule;

template <typename DerivedType, typename BaseType> class TIsDerived
{
public:
    static int AnyFunction(BaseType *base)
    {
        return 1;
    }
    static char AnyFunction(void *other)
    {
        return 0;
    }
    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType *)nullptr))),
    };
};

#define FIND_MODULE(ClassBaseName, ClassName) assert(TIsDerived<ClassBaseName, IModule>::Result);

class IPluginManager
{
public:
    virtual bool LoadPluginConfig()
    {
        return true;
    }
    virtual bool LoadPlugin()
    {
        return true;
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

    template <typename T> T *FindModule()
    {
        IModule *logicModule = FindModule(typeid(T).name());
        if (logicModule)
        {
            if (!TIsDerived<T, IModule>::Result)
                return nullptr;
            T *module = dynamic_cast<T *>(logicModule);
            assert(module != nullptr);
            return module;
        }
        assert(nullptr);
        return nullptr;
    }
    virtual bool ReLoadPlugin(const std::string &pluginDLLName) = 0;

    virtual void Registered(IPlugin *plugin) = 0;

    virtual void UnRegistered(IPlugin *plugin) = 0;

    virtual IPlugin *FindPlugin(const std::string &pluginName) = 0;

    virtual void AddModule(const std::string &moduleName, IModule *pModule) = 0;

    virtual void AddTestModule(const std::string &moduleName, IModule *pModule) = 0;

    virtual void RemoveModule(const std::string &moduleName) = 0;

    virtual IModule *FindModule(const std::string &moduleName) = 0;

    virtual IModule *FindTestModule(const std::string &moduleName) = 0;

    virtual std::list<IModule *> Modules()     = 0;
    virtual std::list<IModule *> TestModules() = 0;

    virtual bool IsStaticPlugin() const = 0;

    virtual std::string GetConfigPath() const                     = 0;
    virtual void        SetConfigPath(const std::string &strPath) = 0;

    virtual IPlugin *GetCurrentPlugin() = 0;
    virtual IModule *GetCurrentModule() = 0;

    virtual void SetCurrentPlugin(IPlugin *pPlugin) = 0;
    virtual void SetCurrentModule(IModule *pModule) = 0;
};
} // namespace Toy

#endif // !IPLUGINMANAGER_H
