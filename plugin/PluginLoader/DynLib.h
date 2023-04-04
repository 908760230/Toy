#ifndef DYNLIB_H
#define DYNLIB_H

#include "Platform.h"
#include <string>

namespace Toy
{
#if TOY_PLATFORM == TOY_PLATFORM_WIN32
#include <Windows.h>
#define DYNLIB_HANDLE       hInstance
#define DYNLIB_LOAD(a)      LoadLibraryExA(a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define DYNLIB_GETSYM(a, b) GetProcAddress(a, b)
#define DYNLIB_UNLOAD(a)    FreeLibrary(a)

struct HINSTANCE__;
typedef struct HINSTANCE__ *hInstance;

#elif TOY_PLATFORM == TOY_PLATFORM_LINUX || TOY_PLATFORM == TOY_PLATFORM_ANDROID
#include <dlfcn.h>
#define DYNLIB_HANDLE       void *
#define DYNLIB_LOAD(a)      dlopen(a, RTLD_LAZY | RTLD_GLOBAL)
#define DYNLIB_GETSYM(a, b) dlsym(a, b)
#define DYNLIB_UNLOAD(a)    dlclose(a)

#elif TOY_PLATFORM == TOY_PLATFORM_APPLE || TOY_PLATFORM == TOY_PLATFORM_APPLE_IOS
#include <dlfcn.h>
#define DYNLIB_HANDLE       void *
#define DYNLIB_LOAD(a)      dlopen(a, RTLD_LOCAL | RTLD_LAZY)
#define DYNLIB_GETSYM(a, b) dlsym(a, b)
#define DYNLIB_UNLOAD(a)    dlclose(a)

#endif

class DynLib
{

public:
    DynLib(const std::string &name)
    {
        mbMain   = false;
        mstrName = name;
#ifdef TOY_DEBUG_MODE
        // mstrName.append("");
#endif

#if TOY_PLATFORM == TOY_PLATFORM_WIN32
        mstrName.append(".dll");
#elif TOY_PLATFORM == TOY_PLATFORM_LINUX || TOY_PLATFORM == TOY_PLATFORM_ANDROID
        mstrName.append(".so");
#elif TOY_PLATFORM == TOY_PLATFORM_APPLE || TOY_PLATFORM == TOY_PLATFORM_APPLE_IOS
        mstrName.append(".so");
#endif

        printf("LoadPlugin:%s\n", mstrName.c_str());
    }

    ~DynLib()
    {
    }

    bool Load();

    bool UnLoad();

    /// Get the name of the library
    const std::string &GetName(void) const
    {
        return mstrName;
    }

    const bool GetMain(void) const
    {
        return mbMain;
    }

    void *GetSymbol(const char *szProcName);

protected:
    std::string mstrName;
    bool        mbMain;

    DYNLIB_HANDLE mInst;
};

} // namespace Toy
#endif