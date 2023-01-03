
#ifndef __GX_GPU_PROGRAM_H__
#define __GX_GPU_PROGRAM_H__

#include "Shader.h"
#include "RenderState.h"

namespace Geoxide {

	struct GpuProgramDesc
	{
		uint32_t numPasses;
	};

	struct GpuPassDesc
	{
		Shader* vertexShader;
		Shader* skinnedVertexShader;
		Shader* fragmentShader;
		RenderStateDesc state;
	};

	struct GpuProgram : Internal
	{
		GpuProgramDesc desc;
		Local<GpuPassDesc> passes;
	};
}

#endif // !__GX_SHADER_H__

