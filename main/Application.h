#ifndef TOY_APPLICATION_H
#define TOY_APPLICATION_H

#include "PluginManager.h"
#include <memory>

namespace Toy
{
class Application
{
public:
    void Init();
    bool Excute();
    void Shutdown();

private:
    std::shared_ptr<IPluginManager> mPluginManager;
};
} // namespace Toy

#endif // ! TOY_APPLICATION_H
