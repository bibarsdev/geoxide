
#ifndef __GX_MESH_H__
#define __GX_MESH_H__

#include "GpuProgram.h"
#include "GpuBuffer.h"
#include "Texture.h"
#include "RenderState.h"

namespace Geoxide {

	enum PrimitiveTopology
	{
		kPrimitiveTopologyPointList,
		kPrimitiveTopologyLineList,
		kPrimitiveTopologyTriangleList,
	};

	struct Material
	{
		Ref<GpuProgram> program;
		std::vector<Ref<Texture>> textures;
		Local<uint8_t> data;
		uint32_t dataSize;
	};

	struct SubMeshDesc
	{
		uint32_t indexStart, indexCount;
		Material* material;
	};

	struct MeshDesc
	{
		GpuBuffer* vBuffer;
		GpuBuffer* iBuffer;
		PrimitiveTopology topology;
		uint32_t subCount;
		bool isSkinned;
	};

	struct Mesh : Internal
	{
	};

}

#endif // !__GX_MESH_H__

