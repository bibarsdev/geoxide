
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

	void Application::onFrameStart(Event*)
	{
	}

	void Application::onFrameEnd(Event*)
	{
	}

	void Application::onQuit(Event* e)
	{
		if (mTraceQuitEvents)
			sLog.trace("Quit Event");

		mWindow->destroyWindow();

		mIsRunning = false;
	}

	void Application::onWindowClose(Event* e)
	{
		if (mTraceWindowCloseEvents)
			sLog.trace("Window Close Event");
	}

	void Application::onWindowResized(WindowResizedEvent* e)
	{
		if (mTraceWindowResizedEvents)
			sLog.trace("Window Resized Event: " + std::to_string(e->getWidth()) + ", " + std::to_string(e->getWidth()));
	}

	void Application::onWindowFocus(Event* e)
	{
		if (mTraceWindowFocusEvents)
			sLog.trace("Window Focus Event");
	}

	void Application::onWindowLostFocus(Event* e)
	{
		if (mTraceWindowLostFocusEvents)
			sLog.trace("Window Lost Focus Event");
	}

	void Application::onWindowMoved(WindowMovedEvent* e)
	{
		if (mTraceWindowMovedEvents)
			sLog.trace("Window Moved Event: " + std::to_string(e->getX()) + ", " + std::to_string(e->getY()));
	}

	void Application::onKeyUp(KeyUpEvent* e)
	{
		if (mTraceKeyUpEvents)
			sLog.trace("Key Up Event: " + std::to_string(e->getKeyCode()));
	}

	void Application::onKeyDown(KeyDownEvent* e)
	{
		if (mTraceKeyDownEvents)
			sLog.trace("Key Down Event: " + std::to_string(e->getKeyCode()) + ", " + std::to_string(e->isRepeat()));
	}

	void Application::onMouseButtonUp(MouseButtonUpEvent* e)
	{
		if (mTraceMouseButtonUpEvents)
			sLog.trace("Mouse Button Up Event: " + std::to_string(e->getButton()));
	}

	void Application::onMouseButtonDown(MouseButtonDownEvent* e)
	{
		if (mTraceMouseButtonDownEvents)
			sLog.trace("Mouse Button Down Event: " + std::to_string(e->getButton()));
	}

	void Application::onMouseWheel(MouseWheelEvent* e)
	{
		if (mTraceMouseWheelEvents)
			sLog.trace("Mouse Wheel Event: " + std::to_string(e->getDelta()));
	}

	void Application::onMouseMoved(MouseMovedEvent* e)
	{
		if (mTraceMouseMovedEvents)
			sLog.trace("Mouse Moved Event: " + 
				std::to_string(e->getX()) + ", " + std::to_string(e->getY()) + ", " +
				std::to_string(e->getXRel()) + ", " + std::to_string(e->getYRel())
			);
	}

}
