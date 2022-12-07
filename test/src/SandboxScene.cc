
#include "SandboxScene.h"

void SandboxScene::prepareScene()
{
	mScnMan->setBackColor(NewVector(0.2f, 0.2f, 0.8f, 1));
	mScnMan->getMainCamera()->setPosition(NewVector(0, 5, 10));

	mResMan->loadModel("bunny.model");
	mResMan->loadModel("lucy.model");
	mResMan->loadModel("teapot.model");

	Mesh* linkStatueMesh = mResMan->loadModel("link_statue.model");

	MeshEntity* linkStatue = new MeshEntity(linkStatueMesh);

	SceneNode* linkStatueNode = mScnMan->getRootNode()->newChild();
	linkStatueNode->setEntity(linkStatue);
}

void SandboxScene::destroyScene()
{
}

void SandboxScene::onFrameStart(FrameEvent*)
{
}

void SandboxScene::onFrameEnd(FrameEvent*)
{
}
