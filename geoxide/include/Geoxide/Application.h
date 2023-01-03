
#ifndef __GX_APPLICATION_H__
#define __GX_APPLICATION_H__

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"
#include "FrameEvent.h"

#include "Scene.h"
#include "SkeletalAnimation.h"

#include "RendererDevice.h"
#include "RendererContext.h"

#include <imgui.h>

namespace Geoxide {

	namespace co = std::chrono;
	using stdclock = co::steady_clock;

	struct TransformComponent
	{
		TransformComponent(
			float3 position = float3(),
			float3 orientation = float3(),
			float3 scaling = float3(1, 1, 1))
			: position(position), orientation(orientation), scaling(scaling) {}

		float3 position;
		float3 orientation;
		float3 scaling;
	};

	struct PointLightComponent : PointLightDesc
	{
		PointLightComponent(cfloat3 position, float radius, cfloat3 color, float brightness)
			: PointLightDesc{ position, radius, color, brightness } {}
	};

	struct RendererComponent
	{
		RendererComponent(Mesh* mesh) : mesh(mesh){}

		Ref<Mesh> mesh;
	};

	struct SkeletalComponent
	{
		SkeletalComponent(Shared<Skeleton>& skeleton) : skeleton(skeleton) { sate.setAnimation(&skeleton->getAnimationByIndex(0)); }

		Shared<Skeleton> skeleton;
		SkeletalAnimationState sate;
	};

	struct ResourcePaths
	{
		std::string shaders;
		std::string materials;
		std::string models;
		std::string textures;
	};

	struct ApplicationInit
	{
		WindowInit window;
		ResourcePaths paths;
	};

	class Application
	{
	protected:
		Application(const std::string& name);
		virtual ~Application();

		void initialize(const ApplicationInit& args);

		void startRendering();
		void stopRendering();

		Scene* newScene(const std::string& name);

		Entity* newModel(const std::string& name);
		Entity* newSkyBox(const std::string& texture);

		void setTraceAllEvents(bool traceAllEvents);

		virtual void onFrameStart(FrameEvent*);
		virtual void onFrameEnd(FrameEvent*);
		virtual void onImGui(FrameEvent*);
		virtual void onQuit(Event*);
		virtual void onWindowClose(Event*); // TODO: Never gets called. Remove or fix
		virtual void onWindowResized(WindowResizedEvent*);
		virtual void onWindowFocus(Event*);
		virtual void onWindowLostFocus(Event*);
		virtual void onWindowMoved(WindowMovedEvent*);
		virtual void onKeyUp(KeyUpEvent*);
		virtual void onKeyDown(KeyDownEvent*);
		virtual void onMouseButtonUp(MouseButtonUpEvent*);
		virtual void onMouseButtonDown(MouseButtonDownEvent*);
		virtual void onMouseWheel(MouseWheelEvent*);
		virtual void onMouseMoved(MouseMovedEvent*);

	protected:
		Local<Window> mWindow;

		Ref<RendererDevice> mGfxDev;
		Ref<RendererContext> mGfxCtx;

		std::unordered_map<std::string, Ref<GpuProgram>> mPrograms;
		std::unordered_map<std::string, std::pair<Ref<GpuBuffer>, Ref<GpuBuffer>>> mMeshDatas;
		std::unordered_map<std::string, Ref<Texture>> mTextures;
		std::unordered_map<std::string, Ref<Mesh>> mMeshes;
		std::unordered_map<std::string, Shared<Material>> mMaterials;
		std::unordered_map<std::string, Shared<Skeleton>> mSkeletons;

		Scene* mCurrentScene;

		SharedLibrary mRendererLib;

		bool mIsRunning;
		bool mTraceQuitEvents;
		bool mTraceWindowCloseEvents;
		bool mTraceWindowResizedEvents;
		bool mTraceWindowFocusEvents;
		bool mTraceWindowLostFocusEvents;
		bool mTraceWindowMovedEvents;
		bool mTraceKeyUpEvents;
		bool mTraceKeyDownEvents;
		bool mTraceMouseButtonUpEvents;
		bool mTraceMouseButtonDownEvents;
		bool mTraceMouseWheelEvents;
		bool mTraceMouseMovedEvents;

		bool mVsync;

	private:
		void initializeRenderer();
		void renderOneFrame();
		void createCubeMesh();
		void createPlaneMesh();

		GpuProgram* loadShader(const std::string& name);
		Texture* loadTexture(const std::string& name);
		Material* loadMaterial(const std::string& name);

	private:
		std::string mName;
		ResourcePaths mPaths;
		bool mHasInitialized;
	};

}

#endif // __GX_APPLICATION_H__
