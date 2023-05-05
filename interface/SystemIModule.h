#ifndef TOY_SYSTEMIMODULE_H
#define TOY_SYSTEMIMODULE_H
#include "IModule.h"

namespace Toy
{

class SystemIModule : public IModule
{
public:
    virtual void run()       = 0;
    virtual bool pollEvent() = 0;
    virtual void resize(int w, int h) = 0;
    // virtual void processID() = 0;
    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

protected:
    int m_width  = 800;
    int m_height = 600;
};
} // namespace Toy

#endif