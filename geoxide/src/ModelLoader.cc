
#include "Geoxide/ModelLoader.h"

#define WRITE_BIN(b) file.write((const char*)&b, sizeof(b))
#define WRITE_NULL() file.write((const char*)&kNULL, sizeof(kNULL))
#define WRITE_STR(s) file << s; WRITE_NULL()
#define WRITE_BUFFER(b, size) file.write((const char*)b, size)

#define READ_BIN(b) file.read((char*)&b, sizeof(b))
#define READ_STR(s) std::getline(file, s, '\0')
#define READ_BUFFER(b, size) file.read((char*)b, size)

#define COPY_BIN(b) b = (decltype(b)&)*modelData; modelData += sizeof(decltype(b))
#define COPY_STR(s) s = (const char*)modelData; modelData += s.size()
#define COPY_BUFFER(b, size) std::copy(modelData, modelData + size, b); modelData += size

namespace Geoxide {
	
	constexpr int kMagicNumber = 'DMXG';
	constexpr uint8_t kNULL = 0;

	size_t ModelLoader::Read(const std::string& filepath, ModelData& model)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open())
			return 0;

		int magicNumber = 0;

		READ_BIN(magicNumber);

		if (magicNumber != kMagicNumber)
		{
			Log::Error("\'" + filepath + "\' is not a mesh file");
			return 0;
		}

		READ_BIN(model.desc);

		model.vertexData = new uint8_t[model.desc.vertexDataSize];
		model.indexData = new uint8_t[model.desc.indexDataSize];

		READ_BUFFER(model.vertexData, model.desc.vertexDataSize);
		READ_BUFFER(model.indexData, model.desc.indexDataSize);

		model.subMeshes = new ModelData::SubMesh[model.desc.numSubMeshes];

		for (uint32_t i = 0; i < model.desc.numSubMeshes; i++)
		{
			ModelData::SubMesh& subMesh = model.subMeshes[i];

			READ_BIN(subMesh.desc);

			READ_STR(subMesh.material);
		}

		for (uint32_t i = 0; i < model.desc.numBones; i++)
		{
			READ_BIN(model.bones[i]);
		}

		for (uint32_t i = 0; i < model.desc.numAnimations; i++)
		{
			ModelData::Animation& animation = model.animations[i];

			READ_BIN(animation.desc);

			READ_STR(animation.name);

			READ_BUFFER(animation.matrices, animation.desc.numFrames * model.desc.numBones * sizeof(Matrix));
		}

		size_t size = file.tellg();

		return size;
	}

	size_t ModelLoader::Write(const ModelData& model, const std::string& filepath)
	{
		std::ofstream file(filepath, std::ios::binary);

		if (!file.is_open())
			return 0;

		WRITE_BIN(kMagicNumber);

		WRITE_BIN(model.desc);

		WRITE_BUFFER(model.vertexData, model.desc.vertexDataSize);
		WRITE_BUFFER(model.indexData, model.desc.indexDataSize);

		for (uint32_t i = 0; i < model.desc.numSubMeshes; i++)
		{
			ModelData::SubMesh& subMesh = model.subMeshes[i];

			WRITE_BIN(subMesh.desc);

			WRITE_STR(subMesh.material);
		}

		for (uint32_t i = 0; i < model.desc.numBones; i++)
		{
			WRITE_BIN(model.bones[i]);
		}

		for (uint32_t i = 0; i < model.desc.numAnimations; i++)
		{
			ModelData::Animation& animation = model.animations[i];

			WRITE_BIN(animation.desc);

			WRITE_STR(animation.name);

			WRITE_BUFFER(animation.matrices, animation.desc.numFrames * model.desc.numBones * sizeof(Matrix));
		}

		size_t size = file.tellp();

		return size;
	}


	void ModelLoader::Free(ModelData& model)
	{
		delete model.vertexData;
		delete model.indexData;
		delete[model.desc.numSubMeshes] model.subMeshes;
	}
}
