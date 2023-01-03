
// TODO: add multi-tasking

#include "Geoxide/MeshConverter.h"
#include "Geoxide/TextureConverter.h"

#define PARSE_OUTPUT_DIR(dest)\
dest = *(++i);\
if (dest.empty())\
{\
	std::cout << "ERROR: No output directory specified!\n";\
	exit(-1);\
}\
if (!dest.empty() && dest.string().back() != '/')\
dest += '/'

#define MATERIAL_COLOR(matkey, var)  \
float4 var = float4(0,0,0,1); \
if (mat->Get(matkey, color) == aiReturn_SUCCESS) \
	var = float4(color.r,color.g,color.b,color.a);

#define MATERIAL_REAL(matkey, var) \
float var = 0; \
if (mat->Get(matkey, real) == aiReturn_SUCCESS) \
	var = real;

#define MATERIAL_TEXTURE(texType, var) \
std::string var; \
if (mat->GetTexture(texType, 0, &str) == aiReturn_SUCCESS) \
	var = str.C_Str();

namespace Geoxide {

	const std::unordered_map<CompressionType, std::string> kCompDict =
	{
		{kCompressionTypeNone, "None"},
		{kCompressionTypeBC3, "BC3"},
		{kCompressionTypeBC4, "BC4"},
		{kCompressionTypeBC5, "BC5"},
	};

	MeshConverter::MeshConverter(const std::vector<std::string>& args) :
		mFlags(aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_PopulateArmatureData),
		mDiffuseCompression(kCompressionTypeNone), mSpecularCompression(kCompressionTypeNone),
		mScene(0), mLastVertexDataSize(0), mLastIndexDataSize(0), mVertexSize(0)
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
			mScene = mImp.ReadFile(file.string(), mFlags);

			if (mScene)
			{
				mCurrentSourceFile = file.string();
				mCurrentModelName = file.filename().replace_extension().string();
				mCurrentSourceDir = file.remove_filename().string();

				if (mScene->HasMeshes())
				{
					aiMesh* mesh = mScene->mMeshes[0];

					mHasPosition = mesh->HasPositions();
					mHasNormals = mesh->HasNormals();
					mHasTexcoords = /*mesh->HasTextureCoords(0)*/ false;
					mHasBones = mesh->HasBones();

					if (!mHasPosition)
					{
						std::cout << "ERROR: file \'" << mCurrentSourceFile << "\' has no positions. Skipped.";
						continue;
					}

					mVertexSize = sizeof(float) * 4;

					if (mHasTexcoords)
					{
						mVertexSize += sizeof(float) * 2;
					}

					if (mHasNormals)
					{
						mVertexSize += sizeof(float) * 3;
					}

					if (mHasBones)
					{
						mVertexSize += sizeof(uint32_t) * 4 + sizeof(float) * 4;

						aiBone* rootBone = mesh->mBones[0];
						aiNode* rootNode = rootBone->mNode;
						Bone firstBone = {};

						firstBone.invModelMatrix = (float4x4&)rootBone->mOffsetMatrix;
						firstBone.localMatrix = (float4x4&)rootNode->mTransformation;

						firstBone.parentIndex = -1;

						mBoneIdsMap[rootNode->mName.C_Str()] = 0;
						mBones.push_back(firstBone);
						
						for (uint32_t i = 1; i < mesh->mNumBones; i++)
						{
							aiBone* bone = mesh->mBones[i];
							aiNode* node = bone->mNode;
							Bone tempBone = {};

							aiNode* ParentNode = node->mParent;

							/*while (mBoneIdsMap.find(ParentNode->mName.C_Str()) == mBoneIdsMap.end())
							{
								tempBone.localMatrix = MatrixMultiply(tempBone.localMatrix, (Matrix&)ParentNode->mTransformation);
								ParentNode = ParentNode->mParent;
							}*/

							tempBone.parentIndex = mBoneIdsMap.at(ParentNode->mName.C_Str());

							tempBone.invModelMatrix = (float4x4&)bone->mOffsetMatrix;

							//tempBone.localMatrix = (float4x4&)(( aiMatrix4x4&)mBones[tempBone.parentIndex].invModelMatrix * bone->mOffsetMatrix.Inverse() );

							tempBone.localMatrix = (float4x4&)node->mTransformation;

							mBoneIdsMap[bone->mName.C_Str()] = mBones.size();
							mBones.push_back(tempBone);
						}

						//processBoneNode(mesh->mBones[0]->mArmature, -1);
					}

				}

				processNode(mScene->mRootNode);

				for (uint32_t i = 0; i < mScene->mNumAnimations; i++)
				{
					processAnimation(mScene->mAnimations[i]);
				}

				ModelData model = {};

				model.desc.vertexLength = mVertexSize;
				model.desc.vertexDataSize = mVertexData.size();
				model.vertexData = mVertexData.data();

				model.desc.indexLength = sizeof(uint32_t);
				model.desc.indexDataSize = mIndexData.size() * sizeof(mIndexData[0]);
				model.indexData = mIndexData.data();

				model.subMeshes = mSubMeshes.data();
				model.desc.numSubMeshes = mSubMeshes.size();

				model.bones = mBones.data();
				model.desc.numBones = mBones.size();

				model.animations = mAnimations.data();
				model.desc.numAnimations = mAnimations.size();

				std::string outModelFile = mModelsOutputDir.string() + mCurrentModelName + ".model";

				if (ModelLoader::Write(model, outModelFile))
					std::cout << "Written model to file \'" << outModelFile << "\'.\n";
				else
					std::cout << "ERROR: Failed to write model to file \'" << outModelFile << "\'.\n";

				mVertexData.clear();
				mIndexData.clear();
				mSubMeshes.clear();
				mMaterialsDict.clear();
				mTexturesDict.clear();
				mBoneIdsMap.clear();
				mBones.clear();

				for (auto& anim : mAnimations)
				{
					//std::vector<std::map<float, KeyFrame>> tracks;

					for (uint32_t i = 0; i < anim.desc.numTracks; i++)
					{
						//tracks.push_back(std::map<float, KeyFrame>(anim.tracks[i].keyFrames, anim.tracks[i].keyFrames + anim.tracks[i].desc.numKeyFrames));
						delete anim.tracks[i].keyFrames;
					}

					delete anim.tracks;
				}

				mAnimations.clear();

				mLastVertexDataSize = 0;
				mLastIndexDataSize = 0;
				mVertexSize = 0;

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
		aiBone** bones = mesh->mBones;
		uint32_t numBones = mesh->mNumBones;
		float w = 1;

		std::vector<int4> boneIds;
		std::vector<float4> weights;

		if (mHasBones)
		{
			boneIds.resize(numVertices);
			weights.resize(numVertices);

			for (uint32_t i = 0; i < numVertices; i++)
			{
				uint32_t vertexBoneCount = 0;

				for (uint32_t f = 0; f < numBones; f++)
				{
					aiBone* bone = bones[f];
					uint32_t numWeights = bone->mNumWeights;

					int boneId = mBoneIdsMap.at(bone->mName.C_Str());

					for (uint32_t j = 0; j < numWeights; j++)
					{
						aiVertexWeight& weight = bone->mWeights[j];

						if (weight.mVertexId == i)
						{
							boneIds[i].v[vertexBoneCount] = boneId;
							weights[i].v[vertexBoneCount] = weight.mWeight;

							vertexBoneCount++;
						}
					}
				}
			}
		}

		for (size_t i = 0; i < numVertices; i++)
		{
			WRITE_BIN(mVertexData, vertices[i], sizeof(vertices[0]));
			WRITE_BIN(mVertexData, w, sizeof(w));

			if (mHasNormals)
			{
				WRITE_BIN(mVertexData, normals[i], sizeof(normals[0]));
			}

			if (mHasTexcoords)
			{
				WRITE_BIN(mVertexData, texCoords[i], sizeof(aiVector2D));
			}

			if (mHasBones)
			{
				WRITE_BIN(mVertexData, boneIds[i], sizeof(boneIds[0]));
				WRITE_BIN(mVertexData, weights[i], sizeof(weights[0]));
			}
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
		auto& iter = mMaterialsDict.find(materialIndex);

		if (iter != mMaterialsDict.end())
		{
			subMesh.material = iter->second;
		}
		else
		{
			subMesh.material = processMaterial(materialIndex);
		}

		mSubMeshes.push_back(subMesh);
	}

	void MeshConverter::processAnimation(aiAnimation* animation)
	{
		mAnimations.push_back(ModelData::Animation());

		ModelData::Animation& anim = mAnimations.back();
		anim.name = animation->mName.C_Str();
		anim.desc.duration = animation->mDuration / animation->mTicksPerSecond;
		anim.desc.numTracks = mBones.size();

		anim.tracks = new ModelData::AnimationTrack[anim.desc.numTracks]{};

		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			aiNodeAnim* node = animation->mChannels[i];

			uint32_t channelID = 0;

			auto& iter = mBoneIdsMap.find(node->mNodeName.C_Str());

			if (iter == mBoneIdsMap.end())
			{
				continue;
			}
			else
			{
				channelID = iter->second;
			}

			assert(node->mNumPositionKeys == node->mNumRotationKeys && node->mNumRotationKeys == node->mNumScalingKeys);

			aiVectorKey* positionKeys = node->mPositionKeys;
			aiQuatKey* quaternionKeys = node->mRotationKeys;
			aiVectorKey* scalingKeys = node->mScalingKeys;

			auto& track = anim.tracks[channelID];
			track.desc.numKeyFrames = node->mNumPositionKeys;
			track.keyFrames = new std::pair<float, KeyFrame>[track.desc.numKeyFrames];

			for (uint32_t f = 0; f < node->mNumPositionKeys; f++)
			{
				assert(positionKeys[f].mTime == quaternionKeys[f].mTime && quaternionKeys[f].mTime == scalingKeys[f].mTime);

				track.keyFrames[f].first = positionKeys[f].mTime / animation->mTicksPerSecond;

				track.keyFrames[f].second.position = (float3&)positionKeys[f].mValue;
				track.keyFrames[f].second.quaternion = quat(quaternionKeys[f].mValue.x, quaternionKeys[f].mValue.y, quaternionKeys[f].mValue.z, quaternionKeys[f].mValue.w);
				track.keyFrames[f].second.scaling = (float3&)scalingKeys[f].mValue;
			}
		}

	}

	std::string MeshConverter::processMaterial(uint32_t matIndex)
	{
		aiMaterial* mat = mScene->mMaterials[matIndex];
		MaterialData material;
		aiString str;
		aiColor4D color;
		ai_real real;

		MATERIAL_COLOR(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
		MATERIAL_REAL(AI_MATKEY_SHININESS, SpecularStrength);

		MATERIAL_TEXTURE(aiTextureType_DIFFUSE, DiffuseTexture);
		MATERIAL_TEXTURE(aiTextureType_SPECULAR, SpecularTexture);

		bool hasDiffuseTexture = !DiffuseTexture.empty();
		bool hasSpecularTexture = !SpecularTexture.empty();

		if (hasDiffuseTexture)
		{
			auto& iter = mTexturesDict.find(DiffuseTexture);

			if (iter != mTexturesDict.end())
				material.textures["mainTex"] = iter->second;
			else
				material.textures["mainTex"] = processTexture(DiffuseTexture, mDiffuseCompression);
		}

		if (hasSpecularTexture)
		{
			auto& iter = mTexturesDict.find(SpecularTexture);

			if (iter != mTexturesDict.end())
				material.textures["specularTex"] = iter->second;
			else
				material.textures["specularTex"] = processTexture(SpecularTexture, mSpecularCompression);
		}

		if (hasDiffuseTexture)
		{
			material.shader = "Lit/Texture.hlsl";
		}
		else
		{
			material.shader = "Lit/ColorSpec.hlsl";
			material.vector3s["surfaceColor"] = DiffuseColor.xyz;
			material.floats["specular"] = SpecularStrength;
		}

		std::string matName = mat->GetName().C_Str();
		matName += ".material";

		std::string outMaterialFile = mMaterialsOutputDir.string() + matName;

		if (MaterialLoader::Write(material, outMaterialFile))
			std::cout << "Written material to file \'" << outMaterialFile << "\'.\n";
		else
			std::cout << "ERROR: Failed to write material to file \'" << outMaterialFile << "\'.\n";

		mMaterialsDict[matIndex] = matName;

		return matName;
	}

	std::string MeshConverter::processTexture(const std::string& texFileName, CompressionType comp)
	{
		DDSData texture = {};

		std::string texName = fs::path(texFileName).filename().replace_extension(".texture").string();

		std::vector<std::string> args;
		args.push_back("-o " + mTexturesOutputDir.string() + texName);
		args.push_back("-c " + kCompDict.at(comp));

		TextureConverter texConv(args);
		texConv.start();

		mTexturesDict[texFileName] = texName;

		return texName;
	}

}
