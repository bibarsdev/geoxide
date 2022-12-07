
#include "SandboxScene.h"

#define DEFAULT_CAMERA_POSITION NewVector(0, 5, 10)
#define DEFAULT_CAMERA_ORIENTATION NewVector(0, 0, 0)

void SandboxScene::prepareScene()
{
	mBackColor = NewVector(0.2f, 0.2f, 0.8f, 1);
	mCameraPosition = DEFAULT_CAMERA_POSITION;
	mCameraOrientation = DEFAULT_CAMERA_ORIENTATION;

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
	mScnMan->setBackColor(mBackColor);

	mScnMan->getMainCamera()->setPosition(mCameraPosition);
	mScnMan->getMainCamera()->setOrientation(mCameraOrientation);
}

void SandboxScene::onFrameEnd(FrameEvent*)
{
}

void SandboxScene::onImGui(FrameEvent* e)
{
	if (ImGui::Begin("World"))
	{
		ImGui::ColorPicker4("Background Color", (float*)&mBackColor);
		ImGui::End();
	}

	if (ImGui::Begin("Camera"))
	{
		ImGui::DragFloat("X", &VectorXRef(mCameraPosition));
		ImGui::DragFloat("Y", &VectorYRef(mCameraPosition));
		ImGui::DragFloat("Z", &VectorZRef(mCameraPosition));

		if (ImGui::Button("Reset Position"))
		{
			mCameraPosition = DEFAULT_CAMERA_POSITION;
		}

		ImGui::SliderAngle("Pitch", &VectorXRef(mCameraOrientation));
		ImGui::SliderAngle("Yaw", &VectorYRef(mCameraOrientation));
		ImGui::SliderAngle("Roll", &VectorZRef(mCameraOrientation));

		if (ImGui::Button("Reset Orientation"))
		{
			mCameraOrientation = DEFAULT_CAMERA_ORIENTATION;
		}

		ImGui::End();
	}

}
