
#ifndef __TEST_APPLICATION_H__
#define __TEST_APPLICATION_H__

#include <Geoxide.h>

#define SAFE_DELETE(var) if (var) { delete var; var = 0; }

using namespace Geoxide;

class TestApplication;

class Scene
{
public:
	Scene() : mApp(0), mScnMan(0) {}
	Scene(TestApplication* app);
	virtual ~Scene() = default;

	virtual void prepareScene() = 0;
	virtual void destroyScene() = 0;

	virtual void onFrameStart(FrameEvent*) = 0;
	virtual void onFrameEnd(FrameEvent*) = 0;
	
protected:
	TestApplication* mApp;
	ResourceManager* mResMan;
	SceneManager* mScnMan;
};

class TestApplication : public Application
{
public:
	TestApplication() : mCurrentScene(0), Application("TestApplication") {}
	~TestApplication();

	void start();

	void onFrameStart(FrameEvent*) override;
	void onFrameEnd(FrameEvent*) override;
	void onKeyUp(KeyUpEvent* e) override;

public:
	void enterAndLoadScene();

private:
	std::unordered_map<std::string, Scene*> mScenes;
	Scene* mCurrentScene;
};

#endif
