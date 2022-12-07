
#include "Geoxide/SceneManager.h"

namespace Geoxide {

	SceneManager::~SceneManager() {

		if (mRendererLib)
			UnloadSharedLibrary(mRendererLib);
	}

	void SceneManager::initialize(const SceneManagerInit& args)
	{
		mStartTime = stdclock::now();

		initializeRenderer(args.mWindow);

		mMainCamera = NewLocal<Camera>(
			(float)args.mWindow->getClientWidth(), (float)args.mWindow->getClientHeight(),
			0.1f, 1000.f,
			3.14f / 3.f,
			Camera::kProjectionTypePerspective);

		mSceneBufferStruct = {};
		mLightBufferStruct = {};

		GpuBufferInit bufferDesc = {};

		bufferDesc.shaderBuffer = true;

		bufferDesc.name = "SceneProperties";
		bufferDesc.data = &mSceneBufferStruct;
		bufferDesc.dataSize = sizeof(mSceneBufferStruct);

		mSceneBuffer = mGfx->newBuffer(bufferDesc);

		bufferDesc.name = "LightProperties";
		bufferDesc.data = &mLightBufferStruct;
		bufferDesc.dataSize = sizeof(mLightBufferStruct);

		mLightBuffer = mGfx->newBuffer(bufferDesc);

		bufferDesc.name = "MaterialProperties";
		bufferDesc.data = NewLocal<uint8_t>(1024).get();
		bufferDesc.dataSize = 1024;

		mMaterialBuffer = mGfx->newBuffer(bufferDesc);
	}

	void SceneManager::initializeRenderer(Window* window)
	{
		NewRendererProc NewRenderer = 0;

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

		NewRenderer = (NewRendererProc)GetSharedLibraryProc(mRendererLib, "NewRenderer");

		if (!NewRenderer)
			Log::Fatal("Failed to get the procedure \'NewRenderer\' from shared library \'" + rendererPath + "\'");

		ImGuiContext* imCtx = ImGui::GetCurrentContext();
		ImGuiMemAllocFunc imAlloc = 0;
		ImGuiMemFreeFunc imFree = 0;
		void* imData = 0;

		ImGui::GetAllocatorFunctions(&imAlloc, &imFree, &imData);

		mGfx = NewRenderer(window, imCtx, imAlloc, imFree, imData);

		if (!mGfx)
			Log::Fatal("Renderer failed to initialize");
	}

	void SceneManager::updateSceneBuffer(MatrixConst objectWorld)
	{
		mSceneBufferStruct.WorldViewProj = MatrixMultiply(objectWorld, mViewProjMatrix);
		mSceneBufferStruct.World = objectWorld;
		mSceneBufferStruct.Position = mMainCamera->getPosition();

		mSceneBuffer->write(&mSceneBufferStruct, 0, sizeof(mSceneBufferStruct));
	}

	// TODO: add a lighting system
	void SceneManager::updateLightBuffer(MatrixConst objectWorld)
	{
		mLightBufferStruct.Lights[0].color = NewVector(1, 1, 1, 1);
		mLightBufferStruct.Lights[0].direction = NewVector(0, -1, 0, 0);
		mLightBufferStruct.Lights[0].intensity = 10;

		/*mLightBufferStruct.Lights[1].color = NewVector(1, 0, 0, 1);
		mLightBufferStruct.Lights[1].direction = NewVector(1.f / 3.f, 1.f / 3.f, -1.f / 3.f, 0);
		mLightBufferStruct.Lights[1].intensity = 10;

		mLightBufferStruct.Lights[2].color = NewVector(0, 1, 0, 1);
		mLightBufferStruct.Lights[2].direction = NewVector(-1.f / 3.f, 1.f / 3.f, -1.f / 3.f, 0);
		mLightBufferStruct.Lights[2].intensity = 10;

		mLightBufferStruct.Lights[3].color = NewVector(0, 0, 1, 1);
		mLightBufferStruct.Lights[3].direction = NewVector(0, 1.f / 2.f, 1.f / 2.f, 0);
		mLightBufferStruct.Lights[3].intensity = 10;*/

		mLightBufferStruct.NumLights = 1;

		mLightBuffer->write(&mLightBufferStruct, 0, sizeof(mLightBufferStruct));
	}

	void SceneManager::updateMaterialBuffer(const void* data, uint32_t size)
	{
		mMaterialBuffer->write(data, 0, size);
	}

	void SceneManager::renderOneFrame()
	{
		mGfx->beginScene(mBackColor, 0);

		mMainCamera->updateViewMatrix(mGfx);
		mMainCamera->updateProjectionMatrix(mGfx);

		mViewProjMatrix = MatrixMultiply(mMainCamera->getViewMatrix(), mMainCamera->getProjectionMatrix());

		mRootNode.update();

		mGfx->endScene();
	}

	float SceneManager::getTime() const
	{
		return (float)co::duration_cast<co::milliseconds>(stdclock::now() - mStartTime).count() / 1000.f;
	}

}
