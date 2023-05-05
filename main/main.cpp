#include "Application.h"
#include <clocale>
using namespace Toy;

int main(int argc, char *agrv[])
{
    setlocale(LC_ALL, "");
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