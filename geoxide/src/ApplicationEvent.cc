
#include "Geoxide/Application.h"

namespace Geoxide {

	void Application::setTraceAllEvents(bool traceAllEvents)
	{
		mTraceQuitEvents = traceAllEvents;
		mTraceWindowCloseEvents = traceAllEvents;
		mTraceWindowResizedEvents = traceAllEvents;
		mTraceWindowFocusEvents = traceAllEvents;
		mTraceWindowLostFocusEvents = traceAllEvents;
		mTraceWindowMovedEvents = traceAllEvents;
		mTraceKeyUpEvents = traceAllEvents;
		mTraceKeyDownEvents = traceAllEvents;
		mTraceMouseButtonUpEvents = traceAllEvents;
		mTraceMouseButtonDownEvents = traceAllEvents;
		mTraceMouseWheelEvents = traceAllEvents;
		mTraceMouseMovedEvents = traceAllEvents;
	}

	void Application::onFrameStart(FrameEvent*)
	{
	}

	void Application::onFrameEnd(FrameEvent*)
	{
	}

	void Application::onQuit(Event* e)
	{
		if (mTraceQuitEvents)
			Log::Trace("Quit Event");

		mWindow->destroyWindow();

		mIsRunning = false;
	}

	void Application::onWindowClose(Event* e)
	{
		if (mTraceWindowCloseEvents)
			Log::Trace("Window Close Event");
	}

	void Application::onWindowResized(WindowResizedEvent* e)
	{
		if (mTraceWindowResizedEvents)
			Log::Trace("Window Resized Event: " + std::to_string(e->getWidth()) + ", " + std::to_string(e->getWidth()));
	}

	void Application::onWindowFocus(Event* e)
	{
		if (mTraceWindowFocusEvents)
			Log::Trace("Window Focus Event");
	}

	void Application::onWindowLostFocus(Event* e)
	{
		if (mTraceWindowLostFocusEvents)
			Log::Trace("Window Lost Focus Event");
	}

	void Application::onWindowMoved(WindowMovedEvent* e)
	{
		if (mTraceWindowMovedEvents)
			Log::Trace("Window Moved Event: " + std::to_string(e->getX()) + ", " + std::to_string(e->getY()));
	}

	void Application::onKeyUp(KeyUpEvent* e)
	{
		if (mTraceKeyUpEvents)
			Log::Trace("Key Up Event: " + std::to_string(e->getKeyCode()));
	}

	void Application::onKeyDown(KeyDownEvent* e)
	{
		if (mTraceKeyDownEvents)
			Log::Trace("Key Down Event: " + std::to_string(e->getKeyCode()) + ", " + std::to_string(e->isRepeat()));
	}

	void Application::onMouseButtonUp(MouseButtonUpEvent* e)
	{
		if (mTraceMouseButtonUpEvents)
			Log::Trace("Mouse Button Up Event: " + std::to_string(e->getButton()));
	}

	void Application::onMouseButtonDown(MouseButtonDownEvent* e)
	{
		if (mTraceMouseButtonDownEvents)
			Log::Trace("Mouse Button Down Event: " + std::to_string(e->getButton()));
	}

	void Application::onMouseWheel(MouseWheelEvent* e)
	{
		if (mTraceMouseWheelEvents)
			Log::Trace("Mouse Wheel Event: " + std::to_string(e->getDelta()));
	}

	void Application::onMouseMoved(MouseMovedEvent* e)
	{
		if (mTraceMouseMovedEvents)
			Log::Trace("Mouse Moved Event: " + 
				std::to_string(e->getX()) + ", " + std::to_string(e->getY()) + ", " +
				std::to_string(e->getXRel()) + ", " + std::to_string(e->getYRel())
			);
	}

}
