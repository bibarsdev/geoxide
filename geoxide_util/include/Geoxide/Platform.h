
#ifndef __GX_PLATFORM_H__
#define __GX_PLATFORM_H__

#ifdef GX_PLATFORM_WIN32

#include <windows.h>
#include <comdef.h>

#ifdef _DLL
#define GXAPI __declspec(dllexport)
#else
#define GXAPI __declspec(dllimport)
#endif // GX_BUILD_SHARED

#define LoadSharedLibrary(path) LoadLibrary(path)
#define UnloadSharedLibrary(lib) FreeLibrary(lib)
#define GetSharedLibraryProc(lib, proc) GetProcAddress(lib, proc)

namespace Geoxide { 
	using NativeHandle = HANDLE;
	using SharedLibrary = HMODULE;

	inline std::string GetFormattedMessage(HRESULT hr)
	{
		static char buffer[256];
		
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
			0, hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer,
			256,
			0);

		if (!buffer[0])
			return "Unknown error";

		return buffer;
	}
}

#endif // GX_PLATFORM_WIN32

#ifdef GX_COMPILER_MSVC

#define GXVECTOR __vectorcall

namespace Geoxide {
	class Exception : public std::exception
	{
	public:
		Exception(const std::string& msg) : std::exception(msg.c_str()) {}
	};
}

#define GX_THROW(str) throw Exception(str)
#define GX_FUNC_THROW(str)\
	GX_THROW(std::filesystem::path(__FILE__).filename().string() + "(" + std::to_string(__LINE__) + "): " str)

#endif // GX_COMPILER_MSVC

#endif // !__GX_PLATFORM_H__

