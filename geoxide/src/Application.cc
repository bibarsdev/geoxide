
#include "Geoxide/Application.h"

#define GX_EVENT_CASE(name) case kEvent##name: on##name(ev); break
#define GX_EVENT_CAST_CASE(name) case kEvent##name: on##name((##name##Event*)ev); break

namespace Geoxide {

	Application::Application() : mIsRunning(false), mRootObject(new MovableObject())
	{
		sLog.info("Started");
	}

	Application::~Application()
	{
		delete mWindow;
		delete mGfx;
		delete mRootObject;

		sLog.info("Ended");
	}

	void Application::initialize(const ApplicationInit& args)
	{
		sLog.info("Initializing...");

		initializeWindow(args.window);
		initializeRenderer();

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
		std::string rendererPath = "direct3d11_renderer.dll";
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

		UnloadSharedLibrary(loadedLib);
	}

	void Application::startRendering()
	{
		mIsRunning = true;

		while (mIsRunning) {

			while (Event* ev = mWindow->pollEvent())
			{
				switch (ev->getType())
				{
					GX_EVENT_CASE(WindowClose);
					GX_EVENT_CASE(WindowFocus);
					GX_EVENT_CASE(WindowLostFocus);
					GX_EVENT_CAST_CASE(WindowResized);
					GX_EVENT_CAST_CASE(WindowMoved);
					GX_EVENT_CAST_CASE(KeyUp);
					GX_EVENT_CAST_CASE(KeyDown);
					GX_EVENT_CAST_CASE(MouseButtonUp);
					GX_EVENT_CAST_CASE(MouseButtonDown);
					GX_EVENT_CAST_CASE(MouseWheel);
					GX_EVENT_CAST_CASE(MouseMoved);
				default:
					sLog.error("Captured an unhandled event, type=" + std::to_string(ev->getType()));
					break;
				}
				delete ev;
			}

			renderOneFrame();
		}
	}

	void Application::renderOneFrame()
	{
		updateObjects(mRootObject->getChildren().begin(), mRootObject->getChildren().end(), mRootObject);
	}

	void Application::updateObjects(Object::BufferIterator& begin, Object::BufferIterator& end, Object* parent)
	{
		for (auto& i = begin; i < end; i++)
		{
			auto obj = *i;
			obj->update(mGfx, parent);

			updateObjects(obj->getChildren().begin(), obj->getChildren().end(), parent);
		}
	}

	void Application::addObject(Object* o)
	{
		return mRootObject->addChild(o);
	}

}
