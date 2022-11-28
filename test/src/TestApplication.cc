
#include "TestApplication.h"

#include "MeshScene.h"
#include "SandboxScene.h"

Scene::Scene(TestApplication* app) : mApp(app), mGfx(app->getRenderer()), mMainCamera(app->getMainCamera()) {}

TestApplication::~TestApplication()
{
	for (auto& scene : mScenes)
		delete scene.second;
}

void TestApplication::start()
{
	ApplicationInit appInit;
	appInit.window.title = "Test";
	appInit.window.x = GX_WINDOWPOS_CENTERED;
	appInit.window.y = GX_WINDOWPOS_CENTERED;
	appInit.window.width = 640;
	appInit.window.height = 480;

	initialize(appInit);

	// initialize scenes
	mScenes["Mesh"] = new MeshScene(this);
	mScenes["Sandbox"] = new SandboxScene(this);

	enterAndLoadScene();
}

void TestApplication::onKeyUp(KeyUpEvent* e)
{
	Application::onKeyUp(e);

	switch (e->getKeyCode())
	{
	case kKeyCodeEscape:
		stopRendering();
		mCurrentScene->destroyScene();
		mRootObject->clearChildren();
		enterAndLoadScene();
		break;
	default:
		break;
	}
}

void TestApplication::enterAndLoadScene()
{
	std::string scenesMsg = "Available scenes:";

	for (auto& scene : mScenes)
		scenesMsg = scenesMsg + " " + scene.first;

	Log::Info(scenesMsg);

EnterSceneName:
	std::cout << "Enter scene to load: ";

	std::string selectedScene;
	std::cin >> selectedScene;

	auto& iter = mScenes.find(selectedScene);

	if (iter == mScenes.end())
	{
		Log::Error("Unknown Scene \"" + selectedScene + "\"");
		goto EnterSceneName;
	}

	Scene* loadedScene = iter->second;
	loadedScene->prepareScene();

	mCurrentScene = loadedScene;

	Log::Info("Loaded Scene \"" + selectedScene + "\"");

	startRendering();
}

void TestApplication::onFrameStart(FrameEvent* e)
{
	mCurrentScene->onFrameStart(e);
}

void TestApplication::onFrameEnd(FrameEvent* e)
{
	mCurrentScene->onFrameEnd(e);
}
