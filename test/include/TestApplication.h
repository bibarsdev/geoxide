
#ifndef __TEST_APPLICATION_H__
#define __TEST_APPLICATION_H__

#include <Geoxide.h>

using namespace Geoxide;

class TestApplication : public Application
{
public:
	TestApplication() : Application("TEST") {}
	~TestApplication() {}

	void start();

	void onFrameStart(FrameEvent*) override;
	void onFrameEnd(FrameEvent*) override;
	void onImGui(FrameEvent*) override;
	void onKeyUp(KeyUpEvent* e) override;
	void onMouseMoved(MouseMovedEvent* e) override;

public:
	void prepareScene();

private:
	float4 mBackColor;
	float3 mCameraPosition;
	float3 mCameraOrientation;
	float3 mEntityPosition;
	float3 mEntityOrientation;
	float3 mEntityScaling;
	float3 mLightPosition;
	float4 mLightColor;
	Entity* mMainEntity;
	Entity* mMainLight;
	Entity* mMainSkyBox;
	std::unordered_map<std::string, Entity*> mEntities;
	std::vector<const char*> mEntityNames;
	std::unordered_map<std::string, Entity*> mSkyBoxes;
	std::vector<const char*> mSkyBoxNames;
	float mAnimationSpeed;
};

#endif
