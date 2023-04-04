#ifndef TOY_SYSTEMIMODULE_H
#define TOY_SYSTEMIMODULE_H
#include "IModule.h"

namespace Toy
{

class SystemIModule : public IModule
{
public:
    virtual void run()       = 0;
    virtual void pollEvent() = 0;
    // virtual void processID() = 0;
};
} // namespace Toy

#endif