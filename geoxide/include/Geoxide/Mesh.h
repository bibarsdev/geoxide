
#ifndef __GX_MESH_H__
#define __GX_MESH_H__

#include "Material.h"
#include "MeshData.h"
#include "Movable.h"

namespace Geoxide {

	struct SubMesh
	{
		Material* material;
		uint32_t indexStart, indexCount;
	};

	class Mesh
	{
	public:
		Mesh() : mMeshData(0), mSubMeshes(0), mNumSubMeshes(0) {}

		MeshData* getMeshData() const { return mMeshData; }

		const SubMesh* getSubMeshes() const { return mSubMeshes; }
		uint32_t getNumSubMeshes() const { return mNumSubMeshes; }

	private:
		MeshData* mMeshData;
		SubMesh* mSubMeshes;
		uint32_t mNumSubMeshes;

	private:
		friend class ResourceManager;
	};

}

#endif // !__GX_MESH_H__

