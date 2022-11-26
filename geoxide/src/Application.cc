
#include "Geoxide/Application.h"

#define EVENT_CASE(name) case kEvent##name: on##name(ev); break
#define EVENT_CAST_CASE(name) case kEvent##name: on##name((##name##Event*)ev); break

namespace Geoxide {

	Application::Application() : mBackColor(NewVector(0, 0, 0, 1)), mRootObject(new MovableObject(this)), mIsRunning(false), mHasInitialized(false)
	{
		sLog.info("Started");
		
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
		if (mWindow)
			delete mWindow;
		if (mGfx)
			delete mGfx;
		if (mRootObject)
			delete mRootObject;

		if (mRendererLib)
			UnloadSharedLibrary(mRendererLib);

		sLog.info("Ended");
	}

	void Application::initialize(const ApplicationInit& args)
	{
		sLog.info("Initializing...");

		initializeWindow(args.window);
		initializeRenderer();
		initializeMainCamera();

		mHasInitialized = true;

		sLog.info("Initialized");
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
			sLog.fatal("Failed to load shared library \'" + rendererPath + "\'");

		NewRenderer = (NewRendererProc)GetSharedLibraryProc(loadedLib, "NewRenderer");

		if (!NewRenderer)
			sLog.fatal("Failed to get the procedure \'NewRenderer\' from shared library \'" + rendererPath + "\'");

		mGfx = NewRenderer(mWindow);

		if (!mGfx)
			sLog.fatal("\'NewRenderer\' returned a null pointer");

		if (!mGfx->hasInitialized())
			sLog.fatal("Renderer failed to initialize");
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
			sLog.fatal("Can't start rendering without initializing first");

		if (!mWindow->getVisibility())
			mWindow->setVisibility(true);

		mIsRunning = true;

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
					sLog.error("Captured an unhandled event, type=" + std::to_string(ev->getType()));
					break;
				}
				delete ev;
			}

			onFrameStart(0);
			renderOneFrame();
			onFrameEnd(0);
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
