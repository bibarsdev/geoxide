
#ifndef __GX_SHADER_H__
#define __GX_SHADER_H__

#include "RendererEnum.h"

namespace Geoxide {

	struct ShaderInit
	{
		const void* code;
		uint32_t codeSize;
		ShaderType type;
		const char* entryPoint;
		bool isCodeCompiled;
		uint32_t WVPMatrixOffset, worldMatrixOffset;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;
	};

}

#endif // !__GX_SHADER_H__

