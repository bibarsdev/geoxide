
#include "Geoxide/Application.h"

#define EVENT_CASE(name) case kEvent##name: on##name(ev); break
#define EVENT_CAST_CASE(name) case kEvent##name: on##name((##name##Event*)ev); break

namespace Geoxide {

	Application::Application(const std::string& name) : 
		mName(name),
		mBackColor(NewVector(0, 0, 0, 1)),
		mRootObject(new MovableObject(this)),
		mSharedBuffer(new uint8_t[SHARED_BUFFER_SIZE]),
		mIsRunning(false),
		mHasInitialized(false),
		mAppStartTime(stdclock::now())
	{
		Log::Init(name);

		Log::Info("Started application");
		
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
		if (mWindow) delete mWindow;
		if (mGfx) delete mGfx;
		if (mRootObject) delete mRootObject;
		if (mSharedBuffer) delete mSharedBuffer;

		if (mRendererLib)
			UnloadSharedLibrary(mRendererLib);

		Log::Info("Ended application");
	}

	void Application::initialize(const ApplicationInit& args)
	{
		Log::Info("Initializing application...");

		initializeWindow(args.window);
		initializeRenderer();
		initializeMainCamera();

		mHasInitialized = true;

		Log::Info("Initialized application");
	}

	void Application::initializeWindow(const WindowInit& args)
	{
		mWindow = Window::New(args);
	}

	void Application::initializeRenderer()
	{
		SharedLibrary loadedLib = 0;
		NewRendererProc NewRenderer = 0;

#ifdef GX_PLATFORM_WIN32
		std::string rendererPath = "renderers/direct3d11_renderer.dll";
#endif // GX_PLATFORM_WIN32

		// Load library, call NewRenderer then unload
		loadedLib = LoadSharedLibrary(rendererPath.c_str());

		if (!loadedLib)
			Log::Fatal("Failed to load shared library \'" + rendererPath + "\'");

		NewRenderer = (NewRendererProc)GetSharedLibraryProc(loadedLib, "NewRenderer");

		if (!NewRenderer)
			Log::Fatal("Failed to get the procedure \'NewRenderer\' from shared library \'" + rendererPath + "\'");

		mGfx = NewRenderer(mWindow);

		if (!mGfx)
			Log::Fatal("\'NewRenderer\' returned a null pointer");

		if (!mGfx->hasInitialized())
			Log::Fatal("Renderer failed to initialize");
	}

	void Application::initializeMainCamera()
	{
		mMainCamera = new Camera(
			mWindow->getClientWidth(), mWindow->getClientHeight(),
			0.1f, 1000.f, 
			3.14f / 3.f,
			Camera::kProjectionTypePerspective);
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

		while (mIsRunning) {

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

			currentTime = stdclock::now();
			elapsedTime = (float)co::duration_cast<co::milliseconds>(currentTime - lastTime).count() / 1000.f;
			lastTime = currentTime;

			frameEv.mElapsedTime = elapsedTime;

			onFrameStart(&frameEv);
			renderOneFrame();
			onFrameEnd(&frameEv);
		}
	}

	void Application::stopRendering()
	{
		mIsRunning = false;
	}

	void Application::renderOneFrame()
	{
		mGfx->beginScene(mBackColor, 0);

		mMainCamera->updateViewMatrix(mGfx);
		mMainCamera->updateProjectionMatrix(mGfx);

		mViewProjMatrix = MatrixMultiply(mMainCamera->getViewMatrix(), mMainCamera->getProjectionMatrix());

		updateObjects(mRootObject->getChildren().begin(), mRootObject->getChildren().end(), mRootObject);
		
		mGfx->endScene();
	}

	void Application::updateObjects(Object::BufferIterator& begin, Object::BufferIterator& end, Object* parent)
	{
		for (auto& i = begin; i < end; i++)
		{
			auto obj = *i;
			obj->update(parent);

			updateObjects(obj->getChildren().begin(), obj->getChildren().end(), parent);
		}
	}

}
