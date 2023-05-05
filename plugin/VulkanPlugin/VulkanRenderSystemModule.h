#ifndef TOY_SYSTEMWINMODULE_H
#define TOY_SYSTEMWINMODULE_H

#include "RenderSystemIModule.h"
#include "SystemWinIModule.h"
#include "VulkanInstance.h"
#include <memory>

namespace Toy
{
class VulkanRenderSystemModule : public RenderSystemIModule
{
public:
    VulkanRenderSystemModule(IPluginManager *p)
    {
        m_bIsExecute   = true;
        pPluginManager = p;
    }
    virtual bool Awake() override;
    virtual bool Init() override;
    virtual bool AfterInit() override;
    virtual bool Execute() override;

private:
    VulkanInstance m_vulkanInstance;
    SystemWinIModule *m_systemWinModule = nullptr;
};
} // namespace Toy

#endif // ! TOY_SPDLOGMODULE_H
