
#ifndef __GX_SCENE_MANAGER_H__
#define __GX_SCENE_MANAGER_H__

#include "Camera.h"
#include "EntityRegistry.h"

namespace Geoxide {

	class Scene
	{
	public:
		Scene(uint32_t viewWidth, uint32_t viewHeight);
		~Scene() {}

		void setBackColor(float4 color)
		{
			mBackColor = color;
		}

		EntityRegistry& getRegistery()
		{
			return mRegistry;
		}

		float4& getBackColor()
		{
			return mBackColor;
		}

		Camera& getMainCamera() 
		{
			return mMainCamera;
		}

		Entity* newEntity();

	private:
		EntityRegistry mRegistry;
		float4 mBackColor;
		Camera mMainCamera;
	};

}


#endif // !__GX_RESOURCE_MANAGER_H__
