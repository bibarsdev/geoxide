
#ifndef __GX_SHADER_MANAGER_H__
#define __GX_SHADER_MANAGER_H__

#include "RenderState.h"
#include "Mesh.h"
#include "RendererDevice.h"

namespace Geoxide {

	class ShaderManager
	{
	public:
		static Ref<GpuProgram> newProgramFromFile(RendererDevice* dev, const std::string& filepath);

	public:
		static const RenderStateDesc kDefaultState;
	};

}

#endif // !__ENGINE_DDS_H__
