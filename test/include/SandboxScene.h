
#ifndef __SANDBOX_SCENE_H__
#define __SANDBOX_SCENE_H__

#include "TestApplication.h"

class SandboxScene : public Scene
{
public:
	SandboxScene(TestApplication* app) : Scene(app) {}
	~SandboxScene() {}

	void prepareScene() override;
	void destroyScene() override;

	void onFrameStart(FrameEvent*) override;
	void onFrameEnd(FrameEvent*) override;
	void onImGui(FrameEvent*) override;

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
