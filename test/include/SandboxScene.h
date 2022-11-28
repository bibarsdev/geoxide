
#ifndef __SANDBOX_SCENE_H__
#define __SANDBOX_SCENE_H__

#include "TestApplication.h"

class SandboxScene : public Scene
{
public:
	SandboxScene(TestApplication* app) :
		Scene(app), mColorProgram(0) {}
	~SandboxScene() { destroyScene(); }

	void prepareScene() override;
	void destroyScene() override;

	void onFrameStart(FrameEvent*) override;
	void onFrameEnd(FrameEvent*) override;

protected:
	GpuProgram* mColorProgram;
};

#endif
