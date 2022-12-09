
#ifndef __GX_SCENE_MANAGER_H__
#define __GX_SCENE_MANAGER_H__

#include "Camera.h"
#include "SceneNode.h"

namespace Geoxide {

	namespace co = std::chrono;
	using stdclock = co::steady_clock;

	struct SceneManagerInit
	{
	private:
		mutable Window* mWindow;

	private:
		friend class Application;
		friend class SceneManager;
	};

	class SceneManager
	{
	private:
		struct LightStruct
		{
			Vector direction;
			Vector color;
			float intensity;
			int pad[3];
		};

		struct SceneBufferStruct
		{
			Matrix WorldViewProj;
			Matrix World;
			Vector Position;
		};

		struct LightBufferStruct
		{
			uint32_t NumLights;
			int pad[3];
			LightStruct Lights[4];
		};

	public:
		SceneManager() : 
			mViewProjMatrix(NewMatrixIdentity()), mBackColor(NewVector(0, 0, 0, 1)), mRendererLib(0), mRootNode(this, 0){}
		~SceneManager();

		void initialize(const SceneManagerInit& args);

		void setBackColor(VectorConst color) { mBackColor = color; }

		Renderer* getRenderer() const { return mGfx; }
		Camera* getMainCamera() const { return mMainCamera.get(); }
		GpuBuffer* getSceneBuffer() const { return mSceneBuffer; }
		GpuBuffer* getLightBuffer() const { return mLightBuffer; }
		GpuBuffer* getMaterialBuffer() const { return mMaterialBuffer; }
		MatrixConst getViewProjMatrix() const { return mViewProjMatrix; }
		SceneNode* getRootNode() { return &mRootNode; }

		// Time since initialization in seconds
		float getTime() const;

		void updateSceneBuffer(MatrixConst objectWorld);
		void updateLightBuffer(MatrixConst objectWorld);
		void updateMaterialBuffer(const void* data, uint32_t size);

		void renderOneFrame();

	private:
		void initializeRenderer(Window* window);

	private:
		Local<Camera> mMainCamera;
		GpuBuffer* mSceneBuffer;
		GpuBuffer* mLightBuffer;
		GpuBuffer* mMaterialBuffer;
		Renderer* mGfx;
		SceneNode mRootNode;
		Vector mBackColor;
		Matrix mViewProjMatrix;
		SceneBufferStruct mSceneBufferStruct;
		LightBufferStruct mLightBufferStruct;
		stdclock::time_point mStartTime;
		SharedLibrary mRendererLib;
	};

}


#endif // !__GX_RESOURCE_MANAGER_H__
