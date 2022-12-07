
#ifndef __GX_MODEL_LOADER_H__
#define __GX_MODEL_LOADER_H__

namespace Geoxide {

	struct ModelData
	{
		struct SubMesh
		{
			struct
			{
				uint32_t indexStart, indexCount;
			} desc;

			std::string material;
		};

		struct
		{
			uint32_t compressionType;
			uint32_t vertexLength, indexLength;
			uint32_t vertexDataSize, indexDataSize;
			uint32_t numSubMeshes;
			uint32_t numBones;
			uint32_t numAnimations;
			uint32_t numBlendShapes;
		} desc;

		void* vertexData, * indexData;
		SubMesh* subMeshes;
	};

	class ModelLoader
	{
	public:
		// returns read bytes size
		// make sure to call ModelLoader::Free() after you're done with model
		static size_t Read(const std::string& filepath, ModelData& model);

		// returns written bytes size
		static size_t Write(const ModelData& model, const std::string& filepath);

		static void Free(ModelData& model);
	};

}

#endif // !__ENGINE_DDS_H__
