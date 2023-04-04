#ifndef TOY_LOGIMODULE_H
#define TOY_LOGIMODULE_H

#include "IModule.h"
#include <string>

namespace Toy
{
struct LogIModule : public IModule
{
public:
    virtual void Debug(const std::string &msg, const std::string &func, int line = 0){};
    virtual void Info(const std::string &msg, const std::string &func, int line = 0){};
    virtual void Warning(const std::string &msg, const std::string &func, int line = 0){};
    virtual void Error(const std::string &msg, const std::string &func, int line = 0){};
    virtual void Fatal(const std::string &msg, const std::string &func, int line = 0){};
};
} // namespace Toy

#endif // !TOY_IMODULE_H
