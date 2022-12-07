
#include "Geoxide/ResourceManager.h"

#include "Geoxide/ModelLoader.h"
#include "Geoxide/DDSLoader.h"
#include "Geoxide/MaterialLoader.h"

namespace Geoxide {

	ResourceManager::~ResourceManager()
	{ 
		freeAllResources();
	}

	void ResourceManager::initialize(const ResourceManagerInit& args)
	{
		mGfx = args.mGfx;

		mShadersDir = args.shadersDir;
		mMaterialsDir = args.materialsDir;
		mTexturesDir = args.texturesDir;
		mModelsDir = args.modelsDir;

		if (!mShadersDir.empty() && mShadersDir.string().back() != '/')
			mShadersDir += '/';

		if (!mMaterialsDir.empty() && mMaterialsDir.string().back() != '/')
			mMaterialsDir += '/';

		if (!mTexturesDir.empty() && mTexturesDir.string().back() != '/')
			mTexturesDir += '/';

		if (!mModelsDir.empty() && mModelsDir.string().back() != '/')
			mModelsDir += '/';

		loadShader("Lambert.hlsl");
		loadShader("LambertDiff.hlsl");

		loadShader("Phong.hlsl");
		loadShader("PhongDiff.hlsl");
		loadShader("PhongSpec.hlsl");
		loadShader("PhongDiffSpec.hlsl");
	}

	GpuProgram* ResourceManager::getShader(const std::string& name)
	{
		return _getShader(name).program;
	}

	Texture* ResourceManager::getTexture(const std::string& name)
	{
		auto& iter = mTextures.find(name);

		if (iter == mTextures.end())
			return loadTexture(name + ".texture");

		return iter->second;
	}

	MeshData* ResourceManager::getMeshData(const std::string& name)
	{
		auto& iter = mMeshDatas.find(name);

		if (iter == mMeshDatas.end())
			return loadModel(name + ".model")->getMeshData();

		return iter->second;
	}

	Material* ResourceManager::getMaterial(const std::string& name) 
	{
		auto& iter = mMaterials.find(name);

		if (iter == mMaterials.end())
			return loadMaterial(name + ".material");

		return &iter->second;
	}

	Mesh* ResourceManager::getModel(const std::string& name)
	{
		auto& iter = mMeshes.find(name);

		if (iter == mMeshes.end())
			return loadModel(name + ".model");

		return &iter->second;
	}

	ResourceManager::Shader& ResourceManager::_getShader(const std::string& name)
	{
		auto& iter = mShaders.find(name);

		if (iter == mShaders.end())
			return _loadShader(name + ".hlsl");

		return iter->second;
	}

	ResourceManager::Shader& ResourceManager::_loadShader(const std::string& filename)
	{
		std::string fullPath = mShadersDir.string() + filename;

		std::ifstream file(fullPath);
		std::string fileBuffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		std::string name = std::filesystem::path(filename).replace_extension().string();

		GpuProgramInit desc = {};

		desc.name = name;

		desc.vertex.name = fullPath;
		desc.vertex.code = fileBuffer.c_str();
		desc.vertex.codeSize = fileBuffer.size();
		desc.vertex.entry = "vertex";

		desc.pixel.name = fullPath;
		desc.pixel.code = fileBuffer.c_str();
		desc.pixel.codeSize = fileBuffer.size();
		desc.pixel.entry = "pixel";

		Shader& res = mShaders[name] = createShader(mGfx->newGpuProgram(desc));

		return res;
	}

	GpuProgram* ResourceManager::loadShader(const std::string& filename)
	{
		return _loadShader(filename).program;
	}

	Texture* ResourceManager::loadTexture(const std::string& filename)
	{
		DDSData texData = {};

		DDSLoader::Read(mTexturesDir.string() + filename, texData);

		std::string name = std::filesystem::path(filename).replace_extension().string();

		TextureInit desc = {};

		desc.name = name;

		desc.type = kTextureType2D;
		desc.pixelData = texData.data;
		desc.width = texData.desc.width;
		desc.height = texData.desc.height;
		desc.arraySize = 1;
		desc.mipLevels = texData.desc.mipMapCount;
		desc.renderTo = false;

		desc.format.fourCC = texData.desc.ddspf.fourCC;

		if (!desc.format.fourCC)
		{
			desc.format.bitCount = texData.desc.ddspf.RGBBitCount;
			desc.format.RMask = texData.desc.ddspf.RBitMask;
			desc.format.GMask = texData.desc.ddspf.GBitMask;
			desc.format.BMask = texData.desc.ddspf.BBitMask;
			desc.format.AMask = texData.desc.ddspf.ABitMask;
		}

		Texture* res = mGfx->newTexture(desc);

		DDSLoader::Free(texData);

		mTextures[name] = res;

		return res;
	}

	Material* ResourceManager::loadMaterial(const std::string& filename)
	{
		MaterialData matData = {};

		MaterialLoader::Read(mMaterialsDir.string() + filename, matData);

		std::string name = std::filesystem::path(filename).replace_extension().string();

		Material& res = mMaterials[name] = Material();
		Shader& shader = _getShader(matData.shader);

		res.mState = matData.state;
		res.mProgram = shader.program;
		res.mReflection = shader.ref;

		res.mNumTextures = matData.textures.size();
		res.mTextures = new Texture*[res.mNumTextures];

		res.mDataSize = shader.materialBuffer->getSize();
		res.mData.reset(new uint8_t[res.mDataSize]);

		for (auto& var : matData.vectors)
		{
			const std::string& varName = var.first;
			VectorConst val = var.second;

			auto programVar = shader.materialBuffer->getVariableByName(varName);

			(Vector&)res.mData.get()[programVar->getByteOffset()] = val;
		}

		for (auto& var : matData.floats)
		{
			const std::string& varName = var.first;
			float val = var.second;

			auto programVar = shader.materialBuffer->getVariableByName(varName);

			(float&)res.mData.get()[programVar->getByteOffset()] = val;
		}

		for (auto& var : matData.ints)
		{
			const std::string& varName = var.first;
			int val = var.second;

			auto programVar = shader.materialBuffer->getVariableByName(varName);

			(int&)res.mData.get()[programVar->getByteOffset()] = val;
		}

		for (auto& var : matData.bools)
		{
			const std::string& varName = var.first;
			bool val = var.second;

			auto programVar = shader.materialBuffer->getVariableByName(varName);

			(bool&)res.mData.get()[programVar->getByteOffset()] = val;
		}

		for (auto& tex : matData.textures)
		{
			const std::string& resourceName = tex.first;
			const std::string& texName = tex.second;

			auto resource = res.mProgram->getResourceByName(resourceName);

			res.mTextures[resource->getBindOffset()] = getTexture(texName);
		}

		return &res;
	}

	Mesh* ResourceManager::loadModel(const std::string& filename)
	{
		ModelData modData = {};

		ModelLoader::Read(mModelsDir.string() + filename, modData);

		std::string name = std::filesystem::path(filename).replace_extension().string();

		MeshDataInit desc = {};

		desc.name = name;

		desc.vertexData = modData.vertexData;
		desc.indexData = modData.indexData;

		desc.vertexDataSize = modData.desc.vertexDataSize;
		desc.indexDataSize = modData.desc.indexDataSize;

		desc.vertexLength = modData.desc.vertexLength;
		desc.indexLength = modData.desc.indexLength;

		MeshData* meshData = mGfx->newMeshData(desc); 
		mMeshDatas[name] = meshData;

		Mesh& res = mMeshes[name] = Mesh();

		res.mMeshData = meshData;
		
		res.mNumSubMeshes = modData.desc.numSubMeshes;
		res.mSubMeshes = new SubMesh[res.mNumSubMeshes];

		for (uint32_t i = 0; i < res.mNumSubMeshes; i++)
		{
			ModelData::SubMesh& modSub = modData.subMeshes[i];
			SubMesh& sub = res.mSubMeshes[i];

			sub.indexStart = modSub.desc.indexStart;
			sub.indexCount = modSub.desc.indexCount;

			sub.material = getMaterial(modSub.material);
		}

		ModelLoader::Free(modData);

		return &res;
	}

	void ResourceManager::freeShader(const std::string& name)
	{
		auto& iter = mShaders.find(name);

		if (iter != mShaders.end())
		{
			delete iter->second.program;

			mShaders.erase(iter);
		}
	}

	void ResourceManager::freeTexture(const std::string& name)
	{
		auto& iter = mTextures.find(name);

		if (iter != mTextures.end())
		{
			delete iter->second;

			mTextures.erase(iter);
		}
	}

	void ResourceManager::freeMeshData(const std::string& name)
	{
		auto& iter = mMeshDatas.find(name);

		if (iter != mMeshDatas.end())
		{
			delete iter->second;

			mMeshDatas.erase(iter);
		}
	}

	void ResourceManager::freeMaterial(const std::string& name)
	{
		auto& iter = mMaterials.find(name);

		if (iter != mMaterials.end())
			mMaterials.erase(iter);
	}

	void ResourceManager::freeMesh(const std::string& name)
	{
		auto& iter = mMeshes.find(name);

		if (iter != mMeshes.end())
			mMeshes.erase(iter);
	}

	void ResourceManager::freeAllResources()
	{
		// Renderer resources get freed automatically after the renderer library gets unloaded
		mShaders.clear();
		mTextures.clear();
		mMeshDatas.clear();

		mMaterials.clear();
		mMeshes.clear();
	}

	ResourceManager::Shader ResourceManager::createShader(GpuProgram* program)
	{
		ResourceManager::Shader shader = {};

		shader.program = program;

		shader.sceneBuffer = shader.program->getBufferByName("SceneProperties");
		shader.lightBuffer = shader.program->getBufferByName("LightProperties");
		shader.materialBuffer = shader.program->getBufferByName("MaterialProperties");

		if (shader.sceneBuffer)
			shader.ref.sceneBufferIndex = shader.sceneBuffer->getBindOffset();
		else
			shader.ref.sceneBufferIndex = -1;

		if (shader.lightBuffer)
			shader.ref.lightBufferIndex = shader.lightBuffer->getBindOffset();
		else
			shader.ref.lightBufferIndex = -1;

		if (shader.materialBuffer)
			shader.ref.materialBufferIndex = shader.materialBuffer->getBindOffset();
		else
			shader.ref.materialBufferIndex = -1;

		return shader;
	}
}
