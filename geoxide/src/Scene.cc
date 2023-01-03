
#include "Geoxide/Scene.h"

namespace Geoxide {

	Scene::Scene(uint32_t viewWidth, uint32_t viewHeight) :
		mBackColor(float4(0, 0, 0, 1)),
		mMainCamera(
			viewWidth, viewHeight,
			1.0f, 100.f,
			3.14f / 3.f,
			Camera::kProjectionTypePerspective)
	{
	}

	Entity* Scene::newEntity()
	{
		return new Entity(&mRegistry, mRegistry.newEntity());
	}
}
