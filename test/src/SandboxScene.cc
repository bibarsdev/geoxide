
#include "SandboxScene.h"

#define DEFAULT_CAMERA_POSITION NewVector(0, 5, -10)
#define DEFAULT_CAMERA_ORIENTATION NewVector(0, 0, 0)

#define DEFAULT_ENTITY_POSITION NewVector(0, 0, 0)
#define DEFAULT_ENTITY_ORIENTATION NewVector(0, 0, 0)
#define DEFAULT_ENTITY_SCALING NewVector(1, 1, 1)

void SandboxScene::prepareScene()
{
	mBackColor = NewVector(0.2f, 0.2f, 0.8f, 1);

	mCameraPosition = DEFAULT_CAMERA_POSITION;
	mCameraOrientation = DEFAULT_CAMERA_ORIENTATION;

	mEntityPosition = DEFAULT_ENTITY_POSITION;
	mEntityOrientation = DEFAULT_ENTITY_ORIENTATION;
	mEntityScaling = DEFAULT_ENTITY_SCALING;

	Mesh* bunnyMesh = mResMan->loadModel("bunny.model");
	Mesh* lucyMesh = mResMan->loadModel("lucy.model");
	Mesh* teapotMesh = mResMan->loadModel("teapot.model");
	Mesh* linkStatueMesh = mResMan->loadModel("link_statue.model");

	mEntities["Bunny"] = MeshEntity(bunnyMesh);
	mEntities["Lucy"] = MeshEntity(lucyMesh);
	mEntities["Teapot"] = MeshEntity(teapotMesh);
	mEntities["Link Statue"] = MeshEntity(linkStatueMesh);

	for (auto& entity : mEntities)
	{
		mEntityNames.push_back(entity.first.c_str());
	}

	mNode = mScnMan->getRootNode()->newChild();

	mNode->setEntity(&mEntities.at("Bunny"));
}

void SandboxScene::destroyScene()
{
}

void SandboxScene::onFrameStart(FrameEvent*)
{
	mScnMan->setBackColor(mBackColor);

	mScnMan->getMainCamera()->setPosition(mCameraPosition);
	mScnMan->getMainCamera()->setOrientation(mCameraOrientation);

	MeshEntity* entity = (MeshEntity*)mNode->getEntity();

	entity->setPosition(mEntityPosition);
	entity->setOrientation(mEntityOrientation);
	entity->setScaling(mEntityScaling);
}

void SandboxScene::onFrameEnd(FrameEvent*)
{
}

void SandboxScene::onImGui(FrameEvent* e)
{
	if (ImGui::Begin("World"))
	{
		ImGui::ColorPicker4("Background Color", (float*)&mBackColor);
	}
	ImGui::End();

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

	}
	ImGui::End();

	if (ImGui::Begin("Entity"))
	{
		int itemSelected = 0;
		if (ImGui::ListBox("Entity", &itemSelected, mEntityNames.data(), mEntityNames.size()))
		{
			mNode->setEntity(&mEntities.at(mEntityNames[itemSelected]));
		}

		ImGui::DragFloat("X", &VectorXRef(mEntityPosition));
		ImGui::DragFloat("Y", &VectorYRef(mEntityPosition));
		ImGui::DragFloat("Z", &VectorZRef(mEntityPosition));

		if (ImGui::Button("Reset Position"))
		{
			mEntityPosition = DEFAULT_ENTITY_POSITION;
		}

		ImGui::SliderAngle("Pitch", &VectorXRef(mEntityOrientation));
		ImGui::SliderAngle("Yaw", &VectorYRef(mEntityOrientation));
		ImGui::SliderAngle("Roll", &VectorZRef(mEntityOrientation));

		if (ImGui::Button("Reset Orientation"))
		{
			mEntityOrientation = DEFAULT_ENTITY_ORIENTATION;
		}

		ImGui::DragFloat("X Scaling", &VectorXRef(mEntityScaling));
		ImGui::DragFloat("Y Scaling", &VectorYRef(mEntityScaling));
		ImGui::DragFloat("Z Scaling", &VectorZRef(mEntityScaling));

		if (ImGui::Button("Reset Scaling"))
		{
			mEntityScaling = DEFAULT_ENTITY_SCALING;
		}

	}
	ImGui::End();

}
