
#ifndef __GX_MESH_DATA_H__
#define __GX_MESH_DATA_H__

#include "Geoxide/RendererEnum.h"

namespace Geoxide {

	struct MeshDataInit
	{
		const void* vertexData;
		const void* indexData;
		uint32_t vertexLength;
		uint32_t indexLength;
		uint32_t vertexDataSize;
		uint32_t indexDataSize;
	};

	class MeshData
	{
	public:
		virtual ~MeshData() = default;
	};

}

#endif // !__GX_MESH_DATA_H__

