
#ifndef __GX_MESH_H__
#define __GX_MESH_H__

#include "Material.h"
#include "Movable.h"
#include "MeshData.h"

namespace Geoxide {

	class SubMesh : public Movable
	{
	public:
		SubMesh() :
			indexStart(0), indexCount(0), material(0) {}

		SubMesh(uint32_t indexStart, uint32_t indexCount, Material* material) :
			indexStart(indexStart), indexCount(indexCount), material(material) {}

		void setIndexStart(uint32_t start) { indexStart = start; }
		void setIndexCount(uint32_t count) { indexCount = count; }
		void setMaterial(Material* m) { material = m; }

		uint32_t getIndexStart() const { return indexStart; }
		uint32_t getIndexCount() const { return indexCount; }
		Material* getMaterial() const { return material; }

	private:
		uint32_t indexStart, indexCount;
		Material* material;
	};

	class Mesh
	{
	public:
		Mesh() :
			meshData(0), vertexShader(0), primitiveTopology(kPrimitiveTopologyUnknown) {}

		Mesh(MeshData* meshData, Shader* vertexShader, PrimitiveTopology primitiveTopology) :
			meshData(meshData), vertexShader(vertexShader), primitiveTopology(primitiveTopology) {}

		const auto& getSubMeshes() const { return subMeshes; }

		const SubMesh& getSubMesh(const std::string& subName) const { return subMeshes.at(subName); }
		const SubMesh& getSubMesh(uint32_t subIndex) const { return subMeshes.begin()->second; }

		SubMesh& getSubMesh(const std::string& subName) { return subMeshes.at(subName); }
		SubMesh& getSubMesh(uint32_t subIndex) { return subMeshes.begin()->second; }

		PrimitiveTopology getPrimitiveTopology() const { return primitiveTopology; }
		MeshData* getMeshData() const { return meshData; }
		Shader* getVertexShader() const { return vertexShader; }

		void addSubMesh(const std::string& name, SubMesh& subMesh) { subMeshes[name] = subMesh; }

	private:
		std::unordered_map<std::string, SubMesh> subMeshes;
		MeshData* meshData;
		Shader* vertexShader;
		PrimitiveTopology primitiveTopology;
	};

}

#endif // !__GX_MESH_H__

