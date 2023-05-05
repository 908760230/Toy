#ifndef TOY_SYSTEMWINMODULE_H
#define TOY_SYSTEMWINMODULE_H

#include "SystemWinIModule.h"

namespace Toy
{
class SystemWinModule : public SystemWinIModule
{
public:
    SystemWinModule(IPluginManager *p)
    {
        m_bIsExecute   = true;
        pPluginManager = p;
    }
    virtual bool Awake() override;
    virtual bool Init() override;
    virtual bool AfterInit() override;
    virtual bool Execute() override;

    virtual void run() override;
    virtual bool pollEvent() override;
    virtual void resize(int w, int h) override;
    virtual HWND windowHandle() const override
    {
        return hwnd;
    };

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    HINSTANCE  hinstance;
    WNDCLASSEX winclass; // this will hold the class we create
    HWND       hwnd;     // generic window handle
    MSG        msg;      // generic message

};
} // namespace Toy

#endif // ! TOY_SPDLOGMODULE_H
