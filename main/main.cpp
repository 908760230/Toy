#include "Application.h"
using namespace Toy;

int main(int argc, char *agrv[])
{
    Application app;
    app.Init();

    while (true)
    {
        app.Excute();
    }
    
    app.Shutdown();
    return 0;
}