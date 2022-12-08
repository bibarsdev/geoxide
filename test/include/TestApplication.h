
#ifndef __TEST_APPLICATION_H__
#define __TEST_APPLICATION_H__

#include <Geoxide.h>

using namespace Geoxide;

class TestApplication : public Application
{
public:
	TestApplication() : Application("TestApplication") {}
	~TestApplication() {}

	void start();

	void onFrameStart(FrameEvent*) override;
	void onFrameEnd(FrameEvent*) override;
	void onImGui(FrameEvent*) override;
	void onKeyUp(KeyUpEvent* e) override;

public:
	void prepareScene();

private:
	Vector mBackColor;
	Vector mCameraPosition;
	Vector mCameraOrientation;
	Vector mEntityPosition;
	Vector mEntityOrientation;
	Vector mEntityScaling;
	std::unordered_map<std::string, MeshEntity> mEntities;
	std::vector<const char*> mEntityNames;
	SceneNode* mNode;
};

#endif
