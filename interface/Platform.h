#ifndef TOY_PLATFORM_H
#define TOY_PLATFORM_H

#define TOY_PLATFORM_UNKNOWN 0
#define TOY_PLATFORM_WIN32   1
#define TOY_PLATFORM_LINUX   2

#define TOY_COMPILER_UNKNOWN 0
#define TOY_COMPILER_GNUC    1
#define TOY_COMPILER_MSVC    2
#define TOY_COMPILER_CLANG   3

#if (defined(__WIN32__) || defined(_WIN32)) && defined(__ANDRIOD__)
#define TOY_COMPILER         TOY_COMPILER_GCC
#define TOY_COMPILER_VERSION 470
#elif defined(_MSC_VER)
#define TOY_COMPILER         TOY_COMPILER_MSVC
#define TOY_COMPILER_VERSION _MSC_VER
#elif defined(__clang__)
#define TOY_COMPILER         TOY_COMPILER_CLANG
#define TOY_COMPILER_VERSION (((__clang_major__)*100) + (__clang_minor__ * 10) + __clang_patchlevel__)
#elif
#define TOY_COMPILER         TOY_COMPILER_GNUC
#define TOY_COMPILER_VERSION (((__GNUC__)*100) + (__cGNUC_MINOR__ * 10) + __GNUC_PATCHLEVEL__)
#else
#pragma error "No known compiler. Abort!"
#endif

#if defined(__WIN32__) || defined(_WIN32)
#include <sdkddkver.h>
#define TOY_PLATFORM TOY_PLATFORM_WIN32
#else
#define TOY_PLATFORM TOY_PLATFORM_LINUX
#endif

#ifdef DYNAMIC_DLL
#ifdef TOY_DLL_EXPORT
#define TOY_API __declspec(dllexport)
#else
#define TOY_API __declspec(dllimport)
#endif
#else
#define TOY_API
#endif

#define GET_CLASS_NAME(className) (#className)

#endif // !TOY_PLATFORM_H
