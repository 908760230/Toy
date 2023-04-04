#ifndef TOY_LOGMODULE_H
#define TOY_LOGMODULE_H

#include "LogIModule.h"

namespace Toy
{
class LogModule : public LogIModule
{
public:
    LogModule(IPluginManager *p)
    {
        m_bIsExecute   = true;
        pPluginManager = p;
    }

    void Debug(const std::string &msg, const std::string &func, int line = 0) override;
    void Info(const std::string &msg, const std::string &func, int line = 0) override;
    void Warning(const std::string &msg, const std::string &func, int line = 0) override;
    void Error(const std::string &msg, const std::string &func, int line = 0) override;
    void Fatal(const std::string &msg, const std::string &func, int line = 0) override;
};
} // namespace Toy

#endif // ! TOY_SPDLOGMODULE_H
