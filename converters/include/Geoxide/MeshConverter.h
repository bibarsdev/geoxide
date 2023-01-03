
#ifndef __MESH_CONVERTER_H__
#define __MESH_CONVERTER_H__

#include "Common.h"

#include <Geoxide/ModelLoader.h>
#include <Geoxide/MaterialLoader.h>
#include <Geoxide/DDSLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Geoxide {

	namespace fs = std::filesystem;

	using namespace Assimp;

	class MeshConverter
	{
	public:
		MeshConverter(const std::vector<std::string>& args);
		~MeshConverter() {}

		void start();

	private:
		void processNode(aiNode* node);
		void processMesh(aiMesh* mesh);
		void processAnimation(aiAnimation* animation);
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
		std::string mCurrentSourceFile;
		std::string mCurrentModelName;
		std::string mCurrentSourceDir;

		std::vector<uint8_t> mVertexData;
		std::vector<uint32_t> mIndexData;
		uint32_t mLastVertexDataSize;
		uint32_t mLastIndexDataSize;
		uint32_t mVertexSize;
		bool mHasPosition = false;
		bool mHasNormals = false;
		bool mHasTexcoords = false;
		bool mHasBones = false;

		std::vector<ModelData::SubMesh> mSubMeshes;
		std::unordered_map<uint32_t, std::string> mMaterialsDict;
		std::unordered_map<std::string, std::string> mTexturesDict;
		std::unordered_map<std::string, uint32_t> mBoneIdsMap;
		std::vector<Bone> mBones;
		std::vector<ModelData::Animation> mAnimations;
		std::vector<std::string> mBoneNames;
	};
}

#endif
