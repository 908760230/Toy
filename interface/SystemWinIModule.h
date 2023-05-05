#ifndef  SYSTEMWINIMODULE_H
#define SYSTEMWINIMODULE_H

#include "SystemIModule.h"
#include <Windows.h>

namespace Toy
{

class SystemWinIModule : public SystemIModule
{
public:
    virtual HWND windowHandle() const = 0;
};

}

#endif // ! SYSTEMWINIMODULE_H
