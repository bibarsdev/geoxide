
#ifndef __GX_PLATFORM_H__
#define __GX_PLATFORM_H__

#ifdef GX_PLATFORM_WIN32
#ifdef _DLL
#define GXAPI __declspec(dllexport)
#else
#define GXAPI __declspec(dllimport)
#endif // GX_BUILD_SHARED
namespace Geoxide { using NativeHandle = HANDLE; using SharedLibrary = HMODULE; }
#define LoadSharedLibrary(path) LoadLibrary(path)
#define UnloadSharedLibrary(lib) FreeLibrary(lib)
#define GetSharedLibraryProc(lib, proc) GetProcAddress(lib, proc)
#endif // GX_PLATFORM_WIN32

#ifdef GX_COMPILER_MSVC
#define GXVECTOR __vectorcall
#endif // GX_COMPILER_MSVC

#endif // !__GX_PLATFORM_H__

