#include "Application.h"
using namespace Toy;

#if TOY_PLATFORM == TOY_PLATFORM_WIN32
#include <Windows.h>
#include <tchar.h>
int _tmain(int argc, _TCHAR *agrv[])
#else
int main(int argc, char *agrv[])
#endif
{
    Application app;
    app.Init();

    bool runing = true;
    while (runing)
    {
        runing = app.Excute();
    }
    
    app.Shutdown();
    return 0;
}