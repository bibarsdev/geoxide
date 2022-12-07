
#ifndef __MESH_CONVERTER_H__
#define __MESH_CONVERTER_H__

#include <Geoxide/ModelLoader.h>
#include <Geoxide/MaterialLoader.h>
#include <Geoxide/DDSLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Geoxide {

	using namespace Assimp;

	namespace fs = std::filesystem;

	class MeshConverter
	{
	public:
		enum CompressionType
		{
			kCompressionTypeNone,
			kCompressionTypeBC3,
			kCompressionTypeBC4,
			kCompressionTypeBC5,
		};

	public:
		MeshConverter(const std::vector<std::string>& args);
		~MeshConverter() {}

		void start();

	private:
		void processNode(aiNode* node);
		void processMesh(aiMesh* mesh);
		std::string processMaterial(uint32_t matIndex);
		std::string processTexture(const std::string& texFileName, CompressionType comp);

	private:
		fs::path mModelsOutputDir, mTexturesOutputDir, mMaterialsOutputDir;
		std::vector<fs::path> mInputFiles;
		uint32_t mFlags;
		CompressionType mDiffuseCompression;
		CompressionType mSpecularCompression;

		Importer mImp;
		const aiScene* mScene;
		std::string mCurrentModelName;
		std::string mCurrentSourceDir;

		std::vector<float> mVertexData;
		std::vector<uint32_t> mIndexData;
		uint32_t mLastVertexDataSize;
		uint32_t mLastIndexDataSize;
		bool mHasPosition;
		bool mHasNormals;
		bool mHasTexcoords;

		std::vector<ModelData::SubMesh> mSubMeshes;
		std::unordered_map<std::string, MaterialData> mMaterials;
		std::unordered_map<uint32_t, std::string> mMaterialsIndexMap;
		std::unordered_map<std::string, DDSData> mTextures;
		std::unordered_map<std::string, std::string> mTexturesNameMap;
	};
}

#endif
