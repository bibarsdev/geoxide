
#include "Geoxide/SceneManager.h"

namespace Geoxide {

	SceneNode* SceneNode::newChild()
	{
		mChildren.push_back(SceneNode(mScnMan, this));

		return &mChildren.back();
	}

	void SceneNode::clearChildren()
	{
		mChildren.clear();
	}

	void SceneNode::update()
	{
		if (mEntity)
			mEntity->update(this);

		for (auto& child : mChildren)
			child.update();
	}

}
