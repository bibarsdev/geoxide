
// TODO: add multi-tasking

#include "Geoxide/MeshConverter.h"

#define STB_DXT_IMPLEMENTATION
#include <stb_dxt.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define PARSE_OUTPUT_DIR(dest)\
dest = *(++i);\
if (dest.empty())\
{\
	std::cout << "ERROR: No output directory specified!\n";\
	exit(-1);\
}\
if (!dest.empty() && dest.string().back() != '/')\
dest += '/'

#define PARSE_COMPRESSION_TYPE(dest) \
std::string compName = *(++i);\
if (compName == "None")\
dest = kCompressionTypeNone;\
else if (compName == "DXT5" || compName == "BC3")\
dest = kCompressionTypeBC3;\
else if (compName == "BC4")\
dest = kCompressionTypeBC4;\
else if (compName == "BC5")\
dest = kCompressionTypeBC5;\
else\
{\
	std::cout << "ERROR: Unknown compression type \'" << compName << "\'! use \"meshconv -h\" for help.\n";\
	exit(-1);\
}

#define MATERIAL_COLOR(matkey, var)  \
Vector var = NewVector(0,0,0,1); \
if (mat->Get(matkey, color) == aiReturn_SUCCESS) \
	var = NewVector(color.r,color.g,color.b,color.a);

#define MATERIAL_REAL(matkey, var) \
float var = 0; \
if (mat->Get(matkey, real) == aiReturn_SUCCESS) \
	var = real;

#define MATERIAL_TEXTURE(texType, var) \
std::string var; \
if (mat->GetTexture(texType, 0, &str) == aiReturn_SUCCESS) \
	var = str.C_Str();

#define BC3_ARGS , 1, STB_DXT_NORMAL

#define BLOCK4X4_COMPRESS(blockType, texelType, copyTexel, type, args, fCC)\
		{\
		uint32_t blockSize = sizeof(blockType);\
		uint32_t widthInBlocks = texture.desc.width / 4;\
		uint32_t heightInBlocks = texture.desc.height / 4;\
		uint32_t pitch = widthInBlocks * blockSize;\
		uint32_t textureSize = pitch * heightInBlocks;\
		uint32_t numBlocks = textureSize / blockSize;\
		blockType* blocks = new blockType[numBlocks];\
		for (uint32_t y = 0; y < heightInBlocks; y++)\
		{\
			for (uint32_t x = 0; x < widthInBlocks; x++)\
			{\
				texelType texel[4][4];\
				Pixel* rawTexel[4] = {\
					rawPixels + texture.desc.width * (y * 4) + x * 4,\
					rawPixels + texture.desc.width * (y * 4 + 1) + x * 4,\
					rawPixels + texture.desc.width * (y * 4 + 2) + x * 4,\
					rawPixels + texture.desc.width * (y * 4 + 3) + x * 4\
				};\
				for (uint32_t yy = 0; yy < 4; yy++)\
				{\
					for (uint32_t xx = 0; xx < 4; xx++)\
					{\
						copyTexel\
					}\
				}\
				stb_compress_##type##_block((uint8_t*)(blocks + y * widthInBlocks + x), (const uint8_t*)&texel args);\
			}\
		}\
		texture.data = blocks;\
		texture.desc.pitchOrLinearSize = pitch;\
		texture.dataSize = textureSize;\
		texture.desc.ddspf.fourCC = fCC;\
		break;\
		}\

#define MESH_PROCESS(type, copyVertex)\
		{\
		for (uint32_t i = 0; i < numVertices; i++)\
		{\
			type vertex;\
			copyVertex\
			mVertexData.write((const char*)&vertex, sizeof(vertex));\
		}\
		break;\
		}\

namespace Geoxide {

	struct Pixel
	{
		uint8_t r, g, b, a;
	};

	struct HalfPixel
	{
		uint8_t r, g;
	};

	using Block8 = uint64_t;
	using Block16 = Vector;

	struct VertexPos
	{
		aiVector3D pos;
		float posW = 1;
	};

	struct VertexPosNormal
	{
		aiVector3D pos;
		float posW = 1;
		aiVector3D normal;
	};

	struct VertexPosTexCoords
	{
		aiVector3D pos;
		float posW = 1;
		aiVector2D texCoords;
	};

	struct VertexPosNormalTexCoords
	{
		aiVector3D pos;
		float posW = 1;
		aiVector3D normal;
		aiVector2D texCoords;
	};

	MeshConverter::MeshConverter(const std::vector<std::string>& args) :
		mFlags(aiProcess_OptimizeMeshes | aiProcess_PreTransformVertices | aiProcess_MakeLeftHanded | aiProcess_JoinIdenticalVertices),
		mDiffuseCompression(kCompressionTypeNone), mSpecularCompression(kCompressionTypeNone),
		mScene(0), mLastVertexDataSize(0), mLastIndexDataSize(0), mVertexSize(0), mVertexType(kVertexTypePos)
	{
		if (args.empty())
		{
			std::cout << "No arguments entered! use \"meshconv -h\" for help.\n";
			exit(-1);
		}
		
		for (auto& i = args.begin(); i < args.end(); i++)
		{
			auto& arg = *i;

			if (arg[0] == '-')
			{
				std::string opt = arg.substr(1);

				if (opt == "mo")
				{
					PARSE_OUTPUT_DIR(mModelsOutputDir);
				}
				else if (opt == "to")
				{
					PARSE_OUTPUT_DIR(mTexturesOutputDir);
				}
				else if (opt == "mao")
				{
					PARSE_OUTPUT_DIR(mMaterialsOutputDir);
				}
				else if (opt == "tr")
				{
					mFlags |= aiProcess_Triangulate;
				}
				else if (opt == "cdt")
				{
					PARSE_COMPRESSION_TYPE(mDiffuseCompression);
				}
				else if (opt == "cst")
				{
					PARSE_COMPRESSION_TYPE(mSpecularCompression);
				}
				else if (opt == "h" || opt == "?" || opt == "help")
				{
					std::cout <<
						"usage: meshconv [options] files\n\n"
						"options:\n"
						"\t-tr                       triangulate faces.\n"
						"\t-cdt TYPE                 compress diffuse textures using TYPE.\n"
						"\t-cst TYPE                 compress specular textures using TYPE.\n"
						"\t-mo DIRECTORY             override models output path with DIRECTORY.\n"
						"\t-to DIRECTORY             override textures output path with DIRECTORY.\n"
						"\t-mao DIRECTORY            override materials output path with DIRECTORY.\n"
						"\t-h, -?, -help             display this message.\n\n"
						"texture compression types: None, DXT5/BC3, BC4, BC5.\n";
				}
				else
				{
					std::cout << "ERROR: Unknown option \'" << arg << "\'! use \"meshconv -h\" for help.\n";
					exit(-1);
				}
			}
			else
			{
				mInputFiles.push_back(arg);
			}

		}
	}

	void MeshConverter::start()
	{
		for (auto& file : mInputFiles)
		{
			mScene = mImp.ReadFile(file.string().c_str(), mFlags);

			if (mScene)
			{
				mCurrentSourceFile = file.string();
				mCurrentModelName = file.filename().replace_extension().string();
				mCurrentSourceDir = file.remove_filename().string();

				if (mScene->HasMeshes())
				{
					aiMesh* mesh = mScene->mMeshes[0];

					bool hasPosition = mesh->HasPositions();
					bool hasNormals = mesh->HasNormals();
					bool hasTexcoords = mesh->HasTextureCoords(0);

					if (!hasPosition)
					{
						std::cout << "ERROR: file \'" << mCurrentSourceFile << "\' has no positions. Skipped.";
						continue;
					}

					if (hasNormals && hasTexcoords)
					{
						mVertexType = kVertexTypePosNormalTexCoords;
						mVertexSize = sizeof(VertexPosNormalTexCoords);
					}
					else if (hasNormals)
					{
						mVertexType = kVertexTypePosNormal;
						mVertexSize = sizeof(VertexPosNormal);
					}
					else if (hasTexcoords)
					{
						mVertexType = kVertexTypePosTexCoords;
						mVertexSize = sizeof(VertexPosTexCoords);
					}
					else
					{
						mVertexType = kVertexTypePos;
						mVertexSize = sizeof(VertexPos);
					}
				}

				processNode(mScene->mRootNode);

				ModelData model = {};

				model.desc.vertexLength = mVertexSize;
				model.desc.vertexDataSize = mVertexData.tellp();
				model.vertexData = new uint8_t[model.desc.vertexDataSize];

				mVertexData.read((char*)model.vertexData, model.desc.vertexDataSize);

				model.desc.indexLength = sizeof(uint32_t);
				model.desc.indexDataSize = mIndexData.size() * sizeof(mIndexData[0]);
				model.indexData = mIndexData.data();

				model.subMeshes = mSubMeshes.data();
				model.desc.numSubMeshes = mSubMeshes.size();

				std::string outModelFile = mModelsOutputDir.string() + mCurrentModelName + ".model";

				if (ModelLoader::Write(model, outModelFile))
					std::cout << "Written model to file \'" << outModelFile << "\'.\n";
				else
					std::cout << "ERROR: Failed to write model to file \'" << outModelFile << "\'.\n";

				for (auto& mat : mMaterials)
				{
					std::string outMaterialFile = mMaterialsOutputDir.string() + mat.first + ".material";

					if (MaterialLoader::Write(mat.second, outMaterialFile))
						std::cout << "Written material to file \'" << outMaterialFile << "\'.\n";
					else
						std::cout << "ERROR: Failed to write material to file \'" << outMaterialFile << "\'.\n";
				}

				for (auto& tex : mTextures)
				{
					std::string outTextureFile = mTexturesOutputDir.string() + tex.first + ".texture";

					if (DDSLoader::Write(tex.second, outTextureFile))
						std::cout << "Written texture to file \'" << outTextureFile << "\'.\n";
					else
						std::cout << "ERROR: Failed to write texture to file \'" << outTextureFile << "\'.\n";

					delete tex.second.data;
				}

				mVertexData.clear();
				mIndexData.clear();
				mSubMeshes.clear();
				mMaterials.clear();
				mMaterialsIndexMap.clear();
				mTextures.clear();
				mTexturesNameMap.clear(); 

				mLastVertexDataSize = 0;
				mLastIndexDataSize = 0;
				mVertexSize = 0;
				mVertexType = kVertexTypePos;

				mImp.FreeScene();
			}
			else
			{
				std::cout << "ERROR: Failed to load file \'" << file << "\'. " << mImp.GetErrorString() << '\n';
			}
		}

	}

	void MeshConverter::processNode(aiNode* node)
	{
		uint32_t numMeshes = node->mNumMeshes;
		uint32_t* meshesInd = node->mMeshes;

		aiMesh** meshes = mScene->mMeshes;

		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = meshes[meshesInd[i]];
			processMesh(mesh);
		}

		uint32_t numChildren = node->mNumChildren;
		aiNode** children = node->mChildren;

		for (uint32_t i = 0; i < numChildren; i++)
		{
			aiNode* child = node->mChildren[i];
			processNode(child);
		}
	}

	void MeshConverter::processMesh(aiMesh* mesh)
	{
		ModelData::SubMesh subMesh = {};

		uint32_t numVertices = mesh->mNumVertices;
		aiVector3D* vertices = mesh->mVertices;
		aiColor4D* colors = mesh->mColors[0];
		aiVector3D* normals = mesh->mNormals;
		aiVector3D* texCoords = mesh->mTextureCoords[0];

		switch (mVertexType)
		{
		case kVertexTypePos:
			MESH_PROCESS(VertexPos, vertex.pos = vertices[i];);
		case kVertexTypePosNormal:
			MESH_PROCESS(VertexPosNormal, vertex.pos = vertices[i]; vertex.normal = normals[i];);
		case kVertexTypePosTexCoords:
			MESH_PROCESS(VertexPosTexCoords, vertex.pos = vertices[i]; vertex.texCoords = (aiVector2D&)texCoords[i];);
		case kVertexTypePosNormalTexCoords:
			MESH_PROCESS(VertexPosNormalTexCoords, vertex.pos = vertices[i]; vertex.normal = normals[i]; vertex.texCoords = (aiVector2D&)texCoords[i];);
		}

		uint32_t numFaces = mesh->mNumFaces;
		aiFace* faces = mesh->mFaces;

		for (uint32_t i = 0; i < numFaces; i++)
		{
			aiFace face = faces[i];
			for (uint32_t f = 0; f < face.mNumIndices; f++)
				mIndexData.push_back(mLastVertexDataSize + face.mIndices[f]);
		}

		size_t indexDataSize = mIndexData.size();

		subMesh.desc.indexStart = mLastIndexDataSize;
		subMesh.desc.indexCount = indexDataSize - mLastIndexDataSize;

		mLastIndexDataSize = indexDataSize;
		mLastVertexDataSize += numVertices;

		uint32_t materialIndex = mesh->mMaterialIndex;
		auto& iter = mMaterialsIndexMap.find(materialIndex);

		if (iter != mMaterialsIndexMap.end())
		{
			subMesh.material = iter->second;
		}
		else
		{
			subMesh.material = processMaterial(materialIndex);
		}

		mSubMeshes.push_back(subMesh);
	}

	std::string MeshConverter::processMaterial(uint32_t matIndex)
	{
		aiMaterial* mat = mScene->mMaterials[matIndex];
		MaterialData material;
		aiString str;
		aiColor4D color;
		ai_real real;

		MATERIAL_COLOR(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
		MATERIAL_COLOR(AI_MATKEY_COLOR_SPECULAR, SpecularColor);
		MATERIAL_COLOR(AI_MATKEY_COLOR_AMBIENT, AmbientColor);

		MATERIAL_REAL(AI_MATKEY_SHININESS, SpecularPower);

		MATERIAL_TEXTURE(aiTextureType_DIFFUSE, DiffuseTexture);
		MATERIAL_TEXTURE(aiTextureType_SPECULAR, SpecularTexture);

		bool hasDiffuseTexture = !DiffuseTexture.empty();
		bool hasSpecularTexture = !SpecularTexture.empty();

		if (hasDiffuseTexture)
		{
			auto& iter = mTexturesNameMap.find(DiffuseTexture);

			if (iter != mTexturesNameMap.end())
				material.textures["DiffuseTexture"] = iter->second;
			else
				material.textures["DiffuseTexture"] = processTexture(DiffuseTexture, mDiffuseCompression);
		}

		if (hasSpecularTexture)
		{
			auto& iter = mTexturesNameMap.find(SpecularTexture);

			if (iter != mTexturesNameMap.end())
				material.textures["SpecularTexture"] = iter->second;
			else
				material.textures["SpecularTexture"] = processTexture(SpecularTexture, mSpecularCompression);
		}

		material.vectors["DiffuseColor"] = DiffuseColor;
		material.vectors["AmbientColor"] = AmbientColor;

		if (SpecularPower > 0.f)
		{
			material.vectors["SpecularColor"] = SpecularColor;
			material.floats["SpecularPower"] = SpecularPower;

			if (hasDiffuseTexture && hasSpecularTexture)
				material.shader = "PhongDiffSpec";
			else if (hasDiffuseTexture)
				material.shader = "PhongDiff";
			else if (hasSpecularTexture)
				material.shader = "PhongSpec";
			else
				material.shader = "Phong";
		}
		else
		{
			if (hasDiffuseTexture)
				material.shader = "LambertDiff";
			else
				material.shader = "Lambert";
		}

		material.state = MaterialLoader::kDefaultState;

		std::string matName = mat->GetName().C_Str();

		mMaterials[matName] = material;
		mMaterialsIndexMap[matIndex] = matName;

		return matName;
	}

	std::string MeshConverter::processTexture(const std::string& texFileName, CompressionType comp)
	{
		DDSData texture = {};

		Pixel* rawPixels = (Pixel*)stbi_load(
			(mCurrentSourceDir + texFileName).c_str(),
			(int*)&texture.desc.width, 
			(int*)&texture.desc.height, 
			0,
			STBI_rgb_alpha
		);

		if (!rawPixels)
		{
			rawPixels = (Pixel*)stbi_load(
				texFileName.c_str(),
				(int*)&texture.desc.width,
				(int*)&texture.desc.height,
				0,
				STBI_rgb_alpha
			);

			if (!rawPixels)
			{
				std::cout << "ERROR: Failed to load image file \"" << texFileName << "\'.\n";
				exit(-1);
			}
		}

		texture.desc.headerSize = sizeof(texture.desc);
		texture.desc.mipMapCount = 1;
		texture.desc.flags = kDDSFlagRequired;

		texture.desc.ddspf.headerSize = sizeof(texture.desc.ddspf);
		texture.desc.ddspf.flags = kDDSPixelFlagRGB | kDDSPixelFlagAlphaPixels;


		if (comp == kCompressionTypeNone)
		{
			texture.desc.flags |= kDDSFlagPitch;

			texture.desc.ddspf.RGBBitCount = sizeof(Pixel) * 8;

			// R8G8B8A8
			texture.desc.ddspf.RBitMask = 0x000000FF;
			texture.desc.ddspf.GBitMask = 0x0000FF00;
			texture.desc.ddspf.BBitMask = 0x00FF0000;
			texture.desc.ddspf.ABitMask = 0xFF000000;
		}
		else
		{
			texture.desc.flags |= kDDSFlagLinearSize;
			texture.desc.ddspf.flags |= kDDSPixelFlagFourCC;
		}

		switch (comp)
		{
		case kCompressionTypeNone:
		{
			uint32_t pixelSize = texture.desc.ddspf.RGBBitCount / 8;
			uint32_t pitch = texture.desc.width * pixelSize;
			uint32_t textureSize = pitch * texture.desc.height;

			texture.data = rawPixels;
			texture.desc.pitchOrLinearSize = pitch;
			texture.dataSize = textureSize;
			break; 
		}
		case kCompressionTypeBC3:
			BLOCK4X4_COMPRESS(Block16, Pixel, texel[yy][xx] = rawTexel[yy][xx]; , dxt, BC3_ARGS, kDDSFourCCDXT5);
		case kCompressionTypeBC4:
			BLOCK4X4_COMPRESS(Block8, uint8_t, texel[yy][xx] = rawTexel[yy][xx].r; , bc4, , kDDSFourCCBC4U);
		case kCompressionTypeBC5:
			BLOCK4X4_COMPRESS(Block16, HalfPixel, texel[yy][xx].r = rawTexel[yy][xx].r; texel[yy][xx].g = rawTexel[yy][xx].g; , bc5, , kDDSFourCCATI2);
		}

		std::string texName = fs::path(texFileName).filename().replace_extension().string();

		mTextures[texName] = texture;
		mTexturesNameMap[texFileName] = texName;

		return texName;
	}

}
