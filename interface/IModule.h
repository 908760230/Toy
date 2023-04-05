#ifndef TOY_IMODULE_H
#define TOY_IMODULE_H

#include <string>
#include "IPluginManager.h"


namespace Toy
{
struct IModule
{
public:
    IModule() : m_bIsExecute(false), pPluginManager(nullptr)
    {
    }

    virtual ~IModule()
    {
    }
    // assign IModule Pointer
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
} // namespace Toy

#endif // !TOY_IMODULE_H
