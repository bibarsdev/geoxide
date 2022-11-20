
#ifndef __GX_APPLICATION_H__
#define __GX_APPLICATION_H__

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"

#include "Object.h"

namespace Geoxide {

	struct ApplicationInit
	{
		WindowInit window;
	};

	class Application
	{
	protected:
		Application();
		virtual ~Application();

		void initialize(const ApplicationInit& args);

		void startRendering();

		void setTraceAllEvents(bool traceAllEvents);

		virtual void onQuit(Event*);
		virtual void onWindowClose(Event*);
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
		ColorRGBA mBackColor;
		Object* mRootObject;
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
		void initializeWindow(const WindowInit& args);
		void initializeRenderer();
		void renderOneFrame();
		void updateObjects(Object::BufferIterator& begin, Object::BufferIterator& end, Object* parent);

	private:
		std::unordered_map<std::string, std::string> mRenderersPaths; // Contains paths for the renderers' libraries
		std::vector<std::string> mAvailableRenderers;
		std::string mCurrentRenderer;
		SharedLibrary mRendererLib;
		bool mHasConfigured;
		GX_DECLARE_LOG("Application");
	};

}

#endif // __GX_APPLICATION_H__
