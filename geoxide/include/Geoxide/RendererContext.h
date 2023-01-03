
#ifndef __GX_RENDERER_CONTEXT_H__
#define __GX_RENDERER_CONTEXT_H__

#include "Mesh.h"

namespace Geoxide {

	struct PointLightDesc
	{
		float3 position;
		float radius;
		float3 color;
		float brightness;
	};

	struct SceneDesc
	{
		float4 clearColor;
		float depth;
		float4x4 viewProj;
		float3 eyePos;
		PointLightDesc* pointLights;
		uint32_t pointLightCount;
	};

	struct GpuCommandList : Internal
	{
	};

	class RendererContext : public Internal
	{
	public:
		virtual void begin(const SceneDesc& scene) = 0;
		virtual void end() = 0;

		virtual void submit(Mesh* mesh, cfloat4x4 world) = 0;
		virtual void submitSkinned(Mesh* mesh, cfloat4x4 world, const float4x4* matrices, uint32_t numMatrices) = 0;

		virtual void flush() = 0;

		virtual void execute(GpuCommandList* list) = 0;
		virtual GpuCommandList* finish() = 0;
	};

}

#endif // !__GX_GPU_COMMAND_LIST_H__


