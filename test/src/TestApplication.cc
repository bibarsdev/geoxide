
#include "TestApplication.h"

void TestApplication::start()
{
	ApplicationInit appInit;
	appInit.window.title = "Test Application";
	appInit.window.x = GX_WINDOWPOS_CENTERED;
	appInit.window.y = GX_WINDOWPOS_CENTERED;
	appInit.window.width = 640;
	appInit.window.height = 480;

	initialize(appInit);

	prepareScene();
	startRendering();
}

void TestApplication::prepareScene()
{
	mBackColor = NewColor(0.52f, 0.81f, 0.92f);

	setTraceAllEvents(true);
}
