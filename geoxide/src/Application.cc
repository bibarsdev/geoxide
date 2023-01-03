
#include "Geoxide/Application.h"

#include "Geoxide/ModelLoader.h"
#include "Geoxide/DDSLoader.h"
#include "Geoxide/MaterialLoader.h"

#include "Geoxide/ShaderManager.h"

#define EVENT_CASE(name) case kEvent##name: on##name(ev); break
#define EVENT_CAST_CASE(name) case kEvent##name: on##name((##name##Event*)ev); break

namespace Geoxide {

	Application::Application(const std::string& name) : 
		mName(name),
		mRendererLib(0),
		mCurrentScene(0),

		mIsRunning(false),
		mHasInitialized(false),
		mTraceQuitEvents(false),
		mTraceWindowCloseEvents(false),
		mTraceWindowResizedEvents(false),
		mTraceWindowFocusEvents(false),
		mTraceWindowLostFocusEvents(false),
		mTraceWindowMovedEvents(false),
		mTraceKeyUpEvents(false),
		mTraceKeyDownEvents(false),
		mTraceMouseButtonUpEvents(false),
		mTraceMouseButtonDownEvents(false),
		mTraceMouseWheelEvents(false),
		mTraceMouseMovedEvents(false),
		mVsync(false)
	{
		Log::Init(mName);

		Log::Info("Started Application");
	}

	Application::~Application()
	{
		ImGui::DestroyContext();

		Log::Info("Ended Application");
	}

	void Application::initialize(const ApplicationInit& args)
	{
		Log::Info("Initializing Application...");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		mWindow = Local<Window>(Window::New(args.window));

		mPaths = args.paths;

		initializeRenderer();

		createCubeMesh();

		mHasInitialized = true;

		Log::Info("Initialized Application");
	}

	void Application::initializeRenderer()
	{
		NewRendererDeviceProc NewRendererDevice = 0;

#ifdef GX_PLATFORM_WIN32
#ifdef _DEBUG
		std::string rendererPath = "renderers/direct3d11_rendererd.dll";
#else
		std::string rendererPath = "renderers/direct3d11_renderer.dll";
#endif _DEBUG
#endif // GX_PLATFORM_WIN32

		mRendererLib = LoadSharedLibrary(rendererPath.c_str());

		if (!mRendererLib)
			Log::Fatal("Failed to load shared library \'" + rendererPath + "\'");

		NewRendererDevice = (NewRendererDeviceProc)GetSharedLibraryProc(mRendererLib, "NewRendererDevice");

		if (!NewRendererDevice)
			Log::Fatal("Failed to get the procedure \'NewRenderer\' from shared library \'" + rendererPath + "\'");

		ImGuiContext* imCtx = ImGui::GetCurrentContext();
		ImGuiMemAllocFunc imAlloc = 0;
		ImGuiMemFreeFunc imFree = 0;
		void* imData = 0;

		ImGui::GetAllocatorFunctions(&imAlloc, &imFree, &imData);

		mGfxDev = NewRendererDevice(mWindow.get(), imCtx, imAlloc, imFree, imData);

		if (!mGfxDev.get())
			Log::Fatal("Renderer failed to initialize");

		mGfxCtx = mGfxDev->getContext();
	}

	Scene* Application::newScene(const std::string& name)
	{
		return new Scene(mWindow->getClientWidth(), mWindow->getClientHeight());
	}

	Entity* Application::newModel(const std::string& name)
	{
		Entity* entity = mCurrentScene->newEntity();

		ModelData modData = {};

		ModelLoader::Read(mPaths.models + name, modData);

		auto& meshData = mMeshDatas[name] = std::pair<Ref<GpuBuffer>, Ref<GpuBuffer>>();

		GpuBufferDesc bufferDesc = {};

		bufferDesc.type = kGpuBufferVertex;
		bufferDesc.size = modData.desc.vertexDataSize;
		bufferDesc.stride = modData.desc.vertexLength;

		meshData.first = mGfxDev->newBuffer(bufferDesc, modData.vertexData);

		bufferDesc.type = kGpuBufferIndex;
		bufferDesc.size = modData.desc.indexDataSize;
		bufferDesc.stride = modData.desc.indexLength;

		meshData.second = mGfxDev->newBuffer(bufferDesc, modData.indexData);

		MeshDesc meshDesc = {};

		meshDesc.vBuffer = meshData.first;
		meshDesc.iBuffer = meshData.second;
		meshDesc.topology = kPrimitiveTopologyTriangleList;
		meshDesc.isSkinned = true;

		meshDesc.subCount = modData.desc.numSubMeshes;
		Local<SubMeshDesc> subs = Local<SubMeshDesc>(new SubMeshDesc[meshDesc.subCount]);
		
		for (uint32_t i = 0; i < meshDesc.subCount; i++)
		{
			ModelData::SubMesh& modSub = modData.subMeshes[i];
			SubMeshDesc& sub = subs.get()[i];

			sub.indexStart = modSub.desc.indexStart;
			sub.indexCount = modSub.desc.indexCount;

			sub.material = loadMaterial(modSub.material);
		}

		Mesh* mesh = mMeshes[name] = mGfxDev->newMesh(meshDesc, subs.get());

		entity->addComponent<TransformComponent>();
		entity->addComponent<RendererComponent>(mesh);

		if (modData.desc.numBones)
		{
			Shared<Skeleton>& skeleton = mSkeletons[name] = NewShared<Skeleton>();

			skeleton->mBones.assign(modData.bones, modData.bones + modData.desc.numBones);

			for (uint32_t i = 0; i < modData.desc.numAnimations; i++)
			{
				ModelData::Animation& animation = modData.animations[i];

				SkeletalAnimation& skeletalAnim = skeleton->mAnimations[animation.name] = SkeletalAnimation(animation.desc.duration);

				for (uint32_t f = 0; f < animation.desc.numTracks; f++)
				{
					skeletalAnim.mSkeleton = skeleton.get();
					skeletalAnim.mTracks.push_back(AnimationTrack());
					skeletalAnim.mTracks.back().mKeyFrames.insert(animation.tracks[f].keyFrames, animation.tracks[f].keyFrames + animation.tracks[f].desc.numKeyFrames);
				}
			}

			entity->addComponent<SkeletalComponent>(skeleton);
		}

		ModelLoader::Free(modData);

		return entity;
	}

	Entity* Application::newSkyBox(const std::string& texture)
	{
		Entity* entity = mCurrentScene->newEntity();
		MeshDesc meshDesc = {};
		GpuBufferDesc bufferDesc = {};

		auto& meshData = mMeshDatas.at("Cube");

		meshDesc.vBuffer = meshData.first;
		meshDesc.iBuffer = meshData.second;
		meshDesc.topology = kPrimitiveTopologyTriangleList;

		meshDesc.subCount = 1;

		SubMeshDesc sub = {};

		sub.indexStart = 0;
		sub.indexCount = meshData.second->desc.size / meshData.second->desc.stride;

		sub.material = (mMaterials["SkyBox." + texture] = NewShared<Material>()).get();

		sub.material->program = loadShader("Unlit/SkyBox.hlsl");

		sub.material->textures.push_back(loadTexture(texture));

		Mesh* mesh = mMeshes["SkyBox." + texture] = mGfxDev->newMesh(meshDesc, &sub);

		entity->addComponent<TransformComponent>(float3(), float3(), float3(500, 500, 500));
		entity->addComponent<RendererComponent>(mesh);

		return entity;
	}

	void Application::startRendering()
	{
		if (!mHasInitialized)
			Log::Fatal("Can't start rendering without initializing first");

		if (!mWindow->getVisibility())
			mWindow->setVisibility(true);

		mIsRunning = true;

		stdclock::time_point lastTime = stdclock::now();
		FrameEvent frameEv(0);

		while (mIsRunning)
		{
			stdclock::time_point currentTime = stdclock::now();
			frameEv.mElapsedTime = (float)co::duration_cast<co::milliseconds>(currentTime - lastTime).count() / 1000.f;
			lastTime = currentTime;

			onFrameStart(&frameEv);
			renderOneFrame();
			onFrameEnd(&frameEv);

			mGfxDev->swapBuffers(mVsync);

			while (Event* ev = mWindow->pollEvent())
			{
				switch (ev->getType())
				{
					EVENT_CASE(Quit);
					EVENT_CASE(WindowClose);
					EVENT_CASE(WindowFocus);
					EVENT_CASE(WindowLostFocus);
					EVENT_CAST_CASE(WindowResized);
					EVENT_CAST_CASE(WindowMoved);
					EVENT_CAST_CASE(KeyUp);
					EVENT_CAST_CASE(KeyDown);
					EVENT_CAST_CASE(MouseButtonUp);
					EVENT_CAST_CASE(MouseButtonDown);
					EVENT_CAST_CASE(MouseWheel);
					EVENT_CAST_CASE(MouseMoved);
				default:
					Log::Error("Captured an unhandled event, type=" + std::to_string(ev->getType()));
					break;
				}
				delete ev;
			}
		}
	}

	void Application::stopRendering()
	{
		mIsRunning = false;
	}

	void Application::renderOneFrame()
	{
		EntityRegistry& entReg = mCurrentScene->getRegistery();
		Camera& camera = mCurrentScene->getMainCamera();
		SceneDesc input = {};

		View<PointLightComponent> lights = entReg.getComponents<PointLightComponent>();

		input.pointLights = lights.begin();
		input.pointLightCount = lights.size();

		camera.updateProjectionMatrix(mGfxDev);
		camera.updateViewMatrix();
		input.viewProj = camera.getViewMatrix() * camera.getProjectionMatrix();

		input.eyePos = camera.getPosition();

		input.clearColor = mCurrentScene->getBackColor();
		input.depth = 1;

		mGfxCtx->begin(input);

		View<entity_t> entites = entReg.getEntities<TransformComponent>();

		for (auto entity : entites)
		{
			TransformComponent* transform = entReg.getComponent<TransformComponent>(entity);

			if (RendererComponent* renderer = entReg.getComponent<RendererComponent>(entity))
			{
				float4x4 translationMatrix = float4x4::translation(transform->position, translationMatrix);
				float4x4 rotationMatrix = float4x4::rotation_pitch_yaw_roll(transform->orientation, rotationMatrix);
				float4x4 scalingMatrix = float4x4::scaling(transform->scaling, scalingMatrix);

				float4x4 world = translationMatrix * rotationMatrix * scalingMatrix;

				/*if (SkeletalComponent* skeleton = entReg.getComponent<SkeletalComponent>(entity))
				{
					const float4x4* frame = skeleton->sate.getCurrentFrameMatrices();

					mGfxCtx->submitSkinned(renderer->mesh, world, frame, skeleton->skeleton->mBones.size());
				}
				else*/
				{
					mGfxCtx->submit(renderer->mesh, world);
				}
			}

			if (PointLightComponent* light = entReg.getComponent<PointLightComponent>(entity))
			{
				light->position = transform->position;
			}
		}

		mGfxCtx->flush();

		mGfxCtx->end();
	}

	void Application::createCubeMesh()
	{
		float vertices[][7] =
		{
			{-1.0f, -1.0f, -1.0f, 1.0f,-1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f, 1.0f,-1.0f,  1.0f, -1.0f},
			{1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f},

			{-1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f},
			{1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{-1.0f,  1.0f, 1.0f, 1.0f,-1.0f,  1.0f, 1.0f},

			{-1.0f, 1.0f, -1.0f, 1.0f,-1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f,  1.0f, 1.0f,-1.0f, 1.0f,  1.0f},
			{1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f},
			{1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f},

			{-1.0f, -1.0f, -1.0f, 1.0f,-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f,  1.0f, 1.0f, 1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f, 1.0f,-1.0f, -1.0f,  1.0f},

			{-1.0f, -1.0f,  1.0f, 1.0f,-1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f, 1.0f,-1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f, -1.0f, 1.0f,-1.0f,  1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f, 1.0f,-1.0f, -1.0f, -1.0f},

			{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f},
			{1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  1.0f, -1.0f},
			{1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.0f,  1.0f},
			{1.0f, -1.0f,  1.0f, 1.0f, 1.0f, -1.0f,  1.0f},
		};

		uint32_t indices[] =
		{
			0,  1,  2,
			0,  2,  3,

			4,  5,  6,
			4,  6,  7,

			8,  9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		};

		MeshDesc meshDesc = {};
		GpuBufferDesc bufferDesc = {};

		auto& meshData = mMeshDatas["Cube"] = std::pair<Ref<GpuBuffer>, Ref<GpuBuffer>>();

		bufferDesc.type = kGpuBufferVertex;
		bufferDesc.size = sizeof(vertices);
		bufferDesc.stride = sizeof(vertices[0]);

		meshData.first = mGfxDev->newBuffer(bufferDesc, vertices);

		bufferDesc.type = kGpuBufferIndex;
		bufferDesc.size = sizeof(indices);
		bufferDesc.stride = sizeof(indices[0]);

		meshData.second = mGfxDev->newBuffer(bufferDesc, indices);

		meshDesc.vBuffer = meshData.first;
		meshDesc.iBuffer = meshData.second;
		meshDesc.topology = kPrimitiveTopologyTriangleList;

		meshDesc.subCount = 1;

		SubMeshDesc sub = {};

		sub.indexStart = 0;
		sub.indexCount = sizeof(indices) / sizeof(indices[0]);

		sub.material = (mMaterials["Cube"] = NewShared<Material>()).get();

		sub.material->program = loadShader("Lit/Color.hlsl");

		static float materialData[] =
		{
			1.0f, 1.0f, 1.0f, 1.0f
		};

		sub.material->dataSize = sizeof(materialData);
		sub.material->data = Local<uint8_t>(new uint8_t[sub.material->dataSize]);

		memcpy(sub.material->data.get(), materialData, sub.material->dataSize);

		mMeshes["Cube"] = mGfxDev->newMesh(meshDesc, &sub);
	}

	void Application::createPlaneMesh()
	{
		float vertices[][7] =
		{
			{-1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, 0.0f},
			{1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f},
			{1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{-1.0f,  1.0f, 0.0f, 1.0f,-1.0f,  1.0f, 0.0f},
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		MeshDesc meshDesc = {};
		GpuBufferDesc bufferDesc = {};

		auto& meshData = mMeshDatas["Plane"] = std::pair<Ref<GpuBuffer>, Ref<GpuBuffer>>();

		bufferDesc.type = kGpuBufferVertex;
		bufferDesc.size = sizeof(vertices);
		bufferDesc.stride = sizeof(vertices[0]);

		meshData.first = mGfxDev->newBuffer(bufferDesc, vertices);

		bufferDesc.type = kGpuBufferIndex;
		bufferDesc.size = sizeof(indices);
		bufferDesc.stride = sizeof(indices[0]);

		meshData.second = mGfxDev->newBuffer(bufferDesc, indices);

		meshDesc.vBuffer = meshData.first;
		meshDesc.iBuffer = meshData.second;
		meshDesc.topology = kPrimitiveTopologyTriangleList;

		meshDesc.subCount = 1;

		SubMeshDesc sub = {};

		sub.indexStart = 0;
		sub.indexCount = sizeof(indices) / sizeof(indices[0]);

		sub.material = (mMaterials["Plane"] = NewShared<Material>()).get();

		sub.material->program = loadShader("Lit/Color.hlsl");

		static float materialData[] =
		{
			1.0f, 1.0f, 1.0f, 1.0f
		};

		sub.material->dataSize = sizeof(materialData);
		sub.material->data = Local<uint8_t>(new uint8_t[sub.material->dataSize]);

		memcpy(sub.material->data.get(), materialData, sub.material->dataSize);

		mMeshes["Plane"] = mGfxDev->newMesh(meshDesc, &sub);
	}

	GpuProgram* Application::loadShader(const std::string& name)
	{
		auto& iter = mPrograms.find(name);

		if (iter != mPrograms.end())
		{
			return iter->second;
		}

		return ShaderManager::newProgramFromFile(mGfxDev, mPaths.shaders + name);
	}

	Texture* Application::loadTexture(const std::string& name)
	{
		auto& iter = mTextures.find(name);

		if (iter != mTextures.end())
		{
			return iter->second;
		}

		DDSData texData = {};

		DDSLoader::Read(mPaths.textures + name, texData);

		TextureDesc desc = {};

		desc.dimension = kTextureDimension2D;
		desc.width = texData.desc.width;
		desc.height = texData.desc.height;
		desc.mipLevels = texData.desc.mipMapCount;
		desc.pitch = texData.desc.pitchOrLinearSize;

		if (texData.desc.detailFlags & kDDSDetailFlagCubeMap)
		{
			desc.cube = true;
			desc.arraySizeOrDepth = 6;
		}
		else
		{
			desc.arraySizeOrDepth = 1;
		}

		if (texData.desc.ddspf.flags & kDDSPixelFlagFourCC)
		{
			switch (texData.desc.ddspf.fourCC)
			{
			case kDDSFourCCDXT5:
				desc.format.type = kDataBC3;
				break;
			case kDDSFourCCATI1:
			case kDDSFourCCBC4U:
				desc.format.type = kDataBC4;
				break;
			case kDDSFourCCATI2:
				desc.format.type = kDataBC5;
				break;
			default:
				Log::Error("Unsupported Four character code. fourCC=" + std::string((const char*)&texData.desc.ddspf.fourCC, 4));
				return 0;
			}
		}
		else
		{
			desc.format.type = kDataByte;
		}

		if (texData.desc.ddspf.flags & kDDSPixelFlagRGB)
		{
			desc.format.bitCount = texData.desc.ddspf.RGBBitCount;
			desc.format.RMask = texData.desc.ddspf.RBitMask;
			desc.format.GMask = texData.desc.ddspf.GBitMask;
			desc.format.BMask = texData.desc.ddspf.BBitMask;
		}

		if (texData.desc.ddspf.flags & kDDSPixelFlagAlphaPixels)
		{
			desc.format.AMask = texData.desc.ddspf.ABitMask;
		}

		Texture* tex = mTextures[name] = mGfxDev->newTexture(desc, texData.data);

		DDSLoader::Free(texData);

		return tex;
	}

	Material* Application::loadMaterial(const std::string& name)
	{
		auto& iter = mMaterials.find(name);

		if (iter != mMaterials.end())
		{
			return iter->second.get();
		}

		MaterialData matData = {};

		MaterialLoader::Read(mPaths.materials + name, matData);

		Shared<Material>& mat = mMaterials[name] = NewShared<Material>();
		GpuProgram* shader = loadShader(matData.shader);

		Shader* fragmentShader = shader->passes.get()[0].fragmentShader;
		auto materialBuffer = fragmentShader->reflection.buffers.at("Material");

		mat->program = shader;

		mat->textures.resize(matData.textures.size());

		mat->dataSize = materialBuffer.size;
		mat->data = Local<uint8_t>(new uint8_t[mat->dataSize]);

		uint8_t* data = mat->data.get();

		for (auto& var : matData.vector4s)
		{
			const std::string& varName = var.first;
			cfloat4 val = var.second;

			auto programVar = materialBuffer.vars[varName];

			(float4&)data[programVar.byteOffset] = val;
		}

		for (auto& var : matData.vector3s)
		{
			const std::string& varName = var.first;
			cfloat3 val = var.second;

			auto programVar = materialBuffer.vars[varName];

			(float3&)data[programVar.byteOffset] = val;
		}

		for (auto& var : matData.vector2s)
		{
			const std::string& varName = var.first;
			cfloat2 val = var.second;

			auto programVar = materialBuffer.vars[varName];

			(float2&)data[programVar.byteOffset] = val;
		}

		for (auto& var : matData.floats)
		{
			const std::string& varName = var.first;
			float val = var.second;

			auto programVar = materialBuffer.vars.at(varName);

			(float&)data[programVar.byteOffset] = val;
		}

		for (auto& var : matData.ints)
		{
			const std::string& varName = var.first;
			int val = var.second;

			auto programVar = materialBuffer.vars.at(varName);

			(int&)data[programVar.byteOffset] = val;
		}

		for (auto& var : matData.bools)
		{
			const std::string& varName = var.first;
			bool val = var.second;

			auto programVar = materialBuffer.vars.at(varName);

			(bool&)data[programVar.byteOffset] = val;
		}

		for (auto& tex : matData.textures)
		{
			const std::string& resourceName = tex.first;
			const std::string& texName = tex.second;

			ShaderResourceDesc resource = fragmentShader->reflection.resources.at(resourceName);

			mat->textures[resource.bindOffset] = loadTexture(texName);
		}
		
		return mat.get();
	}

}
