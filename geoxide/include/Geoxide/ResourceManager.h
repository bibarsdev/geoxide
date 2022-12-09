
#ifndef __GX_RESOURCE_MANAGER_H__
#define __GX_RESOURCE_MANAGER_H__

#include "Renderer.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"

namespace Geoxide {

	namespace fs = std::filesystem;

	struct ResourceManagerInit
	{
		fs::path shadersDir;
		fs::path materialsDir;
		fs::path texturesDir;
		fs::path modelsDir;

	private:
		mutable Renderer* mGfx;

	private:
		friend class Application;
		friend class ResourceManager;
	};

	class ResourceManager
	{
	private:
		struct Shader
		{
			GpuProgram* program;
			Material::Reflection ref;
			const GpuProgramBuffer* sceneBuffer;
			const GpuProgramBuffer* skeletonBuffer;
			const GpuProgramBuffer* lightBuffer;
			const GpuProgramBuffer* materialBuffer;
		};

	public:
		~ResourceManager();

		void initialize(const ResourceManagerInit& args);

		GpuProgram* getShader(const std::string& name);
		Texture* getTexture(const std::string& name);
		MeshData* getMeshData(const std::string& name);
		Material* getMaterial(const std::string& name);
		Mesh* getMesh(const std::string& name);
		Skeleton* getSkeleton(const std::string& name);
		
		GpuProgram* loadShader(const std::string& filename);
		Texture* loadTexture(const std::string& filename);
		Material* loadMaterial(const std::string& filename);
		std::pair<Mesh*, Skeleton*> loadModel(const std::string& filename);

		void freeShader(const std::string& name);
		void freeTexture(const std::string& name);
		void freeMeshData(const std::string& name);
		void freeMaterial(const std::string& name);
		void freeMesh(const std::string& name);
		void freeSkeleton(const std::string& name);

		void freeAllResources();

	private:
		Shader createShader(GpuProgram* program);
		Shader& _loadShader(const std::string& filename);
		Shader& _getShader(const std::string& name);

	private:
		Renderer* mGfx;

		fs::path mShadersDir;
		fs::path mMaterialsDir;
		fs::path mTexturesDir;
		fs::path mModelsDir;

		std::unordered_map<std::string, Shader> mShaders;
		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, MeshData*> mMeshDatas;
		std::unordered_map<std::string, Material> mMaterials;
		std::unordered_map<std::string, Mesh> mMeshes;
		std::unordered_map<std::string, Skeleton> mSkeletons;

	};

}


#endif // !__GX_RESOURCE_MANAGER_H__
