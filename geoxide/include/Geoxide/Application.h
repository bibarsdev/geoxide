
#ifndef __GX_APPLICATION_H__
#define __GX_APPLICATION_H__

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"

#include "Camera.h"
#include "Object.h"

namespace Geoxide {

	struct ApplicationInit
	{
		WindowInit window;
	};

	class Application
	{
	public:
		void setBackColor(VectorConst color) { mBackColor = color; }

		Window* getWindow() const { return mWindow; }
		Renderer* getRenderer() const { return mGfx; }
		Object* getRootObject() const { return mRootObject; }
		Camera* getMainCamera() const { return mMainCamera; }
		MatrixConst getViewProjMatrix() const { return mViewProjMatrix; } // TODO: Add scene manager

	protected:
		Application();
		virtual ~Application();

		void initialize(const ApplicationInit& args);

		void startRendering();
		void stopRendering();

		void setTraceAllEvents(bool traceAllEvents);

		virtual void onFrameStart(Event*);
		virtual void onFrameEnd(Event*);
		virtual void onQuit(Event*);
		virtual void onWindowClose(Event*); // TODO: Never gets called. Remove or fix later
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
		Window* mWindow;
		Renderer* mGfx;
		Object* mRootObject;
		Camera* mMainCamera;
		Vector mBackColor;
		Matrix mViewProjMatrix;
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
		GX_DECLARE_LOG("Application");

	private:
		void initializeWindow(const WindowInit& args);
		void initializeRenderer();
		void initializeMainCamera();
		void renderOneFrame();
		void updateObjects(Object::BufferIterator& begin, Object::BufferIterator& end, Object* parent);

	private:
		std::unordered_map<std::string, std::string> mRenderersPaths; // Contains paths for the renderers' libraries
		std::vector<std::string> mAvailableRenderers;
		std::string mCurrentRenderer;
		SharedLibrary mRendererLib;
		bool mHasInitialized;
	};

}

#endif // __GX_APPLICATION_H__
