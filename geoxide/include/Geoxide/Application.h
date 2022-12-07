
#ifndef __GX_APPLICATION_H__
#define __GX_APPLICATION_H__

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"
#include "FrameEvent.h"

#include "ResourceManager.h"
#include "SceneManager.h"

namespace Geoxide {

	struct ApplicationInit
	{
		WindowInit window;
		ResourceManagerInit resMan;
		SceneManagerInit scnMan;
	};

	class Application
	{
	public:
		ResourceManager* getResourceManager() { return &mResMan; }
		SceneManager* getSceneManager() { return &mScnMan; }

	protected:
		Application(const std::string& name);
		virtual ~Application();

		void initialize(const ApplicationInit& args);

		void startRendering();
		void stopRendering();

		void setTraceAllEvents(bool traceAllEvents);

		virtual void onFrameStart(FrameEvent*);
		virtual void onFrameEnd(FrameEvent*);
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
		Local<Window> mWindow;
		ResourceManager mResMan;
		SceneManager mScnMan;
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

	private:
		std::string mName;
		bool mHasInitialized;
	};

}

#endif // __GX_APPLICATION_H__
