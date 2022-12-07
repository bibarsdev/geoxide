
#include "Geoxide/Application.h"

#define EVENT_CASE(name) case kEvent##name: on##name(ev); break
#define EVENT_CAST_CASE(name) case kEvent##name: on##name((##name##Event*)ev); break

#define SHARED_BUFFER_SIZE 2048

namespace Geoxide {

	Application::Application(const std::string& name) : 
		mName(name),
		mIsRunning(false),
		mHasInitialized(false)
	{
		Log::Init(mName);

		Log::Info("Started Application");
		
		mTraceQuitEvents = false;
		mTraceWindowCloseEvents = false;
		mTraceWindowResizedEvents = false;
		mTraceWindowFocusEvents = false;
		mTraceWindowLostFocusEvents = false;
		mTraceWindowMovedEvents = false;
		mTraceKeyUpEvents = false;
		mTraceKeyDownEvents = false;
		mTraceMouseButtonUpEvents = false;
		mTraceMouseButtonDownEvents = false;
		mTraceMouseWheelEvents = false;
		mTraceMouseMovedEvents = false;
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

		// Initialize Window
		mWindow = Local<Window>(Window::New(args.window));

		// Initialize SceneManager
		args.scnMan.mWindow = mWindow.get();
		mScnMan.initialize(args.scnMan);

		args.resMan.mGfx = mScnMan.getRenderer();
		mResMan.initialize(args.resMan);

		mHasInitialized = true;

		Log::Info("Initialized Application");
	}

	void Application::startRendering()
	{
		if (!mHasInitialized)
			Log::Fatal("Can't start rendering without initializing first");

		if (!mWindow->getVisibility())
			mWindow->setVisibility(true);

		mIsRunning = true;

		stdclock::time_point lastTime;
		stdclock::time_point currentTime;
		float elapsedTime;
		FrameEvent frameEv(0);

		while (mIsRunning) 
		{
			currentTime = stdclock::now();
			elapsedTime = (float)co::duration_cast<co::milliseconds>(currentTime - lastTime).count() / 1000.f;
			lastTime = currentTime;

			frameEv.mElapsedTime = elapsedTime;

			onFrameStart(&frameEv);
			mScnMan.renderOneFrame();
			onFrameEnd(&frameEv);

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

}
