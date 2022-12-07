
#ifndef __GX_SCENE_NODE_H__
#define __GX_SCENE_NODE_H__

#include "Entity.h"

namespace Geoxide {

	class SceneManager;

	class SceneNode
	{
	public:
		void setEntity(Entity* entity) { mEntity = entity; }

		SceneManager* getSceneManager() const { return mScnMan; }
		Entity* getEntity() const { return mEntity; }
		SceneNode* getParent() const { return mParent; }
		const std::vector<SceneNode>& getChildren() const { return mChildren; }

		SceneNode* newChild();
		void clearChildren();
		void update();

	private:
		SceneNode(SceneManager* scnMan, SceneNode* parent) : mScnMan(scnMan), mParent(parent), mEntity(0) {}

	private:
		SceneManager* mScnMan;
		Entity* mEntity;
		SceneNode* mParent;
		std::vector<SceneNode> mChildren;

	private:
		friend class SceneManager;
	};

}


#endif // !__GX_SCENE_NODE_H__
