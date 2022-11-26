
#ifndef __GX_MESH_H__
#define __GX_MESH_H__

#include "Material.h"
#include "MeshData.h"
#include "Movable.h"

namespace Geoxide {

	struct SubMesh
	{
		uint32_t indexStart, indexCount;
		Material* material;
	};

	class Mesh
	{
	public:
		Mesh() : mMeshData(0) {}
		Mesh(MeshData* meshData) : mMeshData(meshData) {}

		const auto& getSubMeshes() const { return mSubMeshes; }
		const SubMesh& getSubMesh(uint32_t i) const { return mSubMeshes[i]; }
		MeshData* getMeshData() const { return mMeshData; }

		void addSubMesh(SubMesh& subMesh) { mSubMeshes.push_back(subMesh); }

	private:
		std::vector<SubMesh> mSubMeshes;
		MeshData* mMeshData;
	};

}

#endif // !__GX_MESH_H__

