#ifndef TOY_SYSTEMWINMODULE_H
#define TOY_SYSTEMWINMODULE_H

#include "SystemIModule.h"

namespace Toy
{
class SystemWinModule : public SystemIModule
{
public:
    SystemWinModule(IPluginManager *p)
    {
        m_bIsExecute   = true;
        pPluginManager = p;
    }

    virtual void run() override;
    virtual void pollEvent() override;
};
} // namespace Toy

#endif // ! TOY_SPDLOGMODULE_H
