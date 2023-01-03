
#include "TestApplication.h"

#define DEFAULT_CAMERA_POSITION float3(0, 10, -20)
#define DEFAULT_CAMERA_ORIENTATION float3(0, 0, 0)

#define DEFAULT_ENTITY_POSITION float3(0, 0, 0)
#define DEFAULT_ENTITY_ORIENTATION float3(0, 0, 0)
#define DEFAULT_ENTITY_SCALING float3(0.1f, 0.1f, 0.1f)

#define DEFAULT_LIGHT_POSITION float3(0, -1, 0)

void TestApplication::start()
{
	ApplicationInit appInit;
	appInit.window.title = "Test";
	appInit.window.x = GX_WINDOWPOS_CENTERED;
	appInit.window.y = GX_WINDOWPOS_CENTERED;
	appInit.window.width = 1280;
	appInit.window.height = 720;
	appInit.paths.shaders = "assets/shaders/";
	appInit.paths.materials = "assets/materials/";
	appInit.paths.models = "assets/models/";
	appInit.paths.textures = "assets/textures/";

	initialize(appInit);

	prepareScene();

	startRendering();
}

void TestApplication::prepareScene()
{
	mCurrentScene = newScene("MainScene");

	mAnimationSpeed = 1.f;

	mBackColor = float4(0.53f, 0.8f, 0.92f, 1);

	mCameraPosition = DEFAULT_CAMERA_POSITION;
	mCameraOrientation = DEFAULT_CAMERA_ORIENTATION;

	mEntityPosition = DEFAULT_ENTITY_POSITION;
	mEntityOrientation = DEFAULT_ENTITY_ORIENTATION;
	mEntityScaling = DEFAULT_ENTITY_SCALING;

	mLightPosition = DEFAULT_LIGHT_POSITION;
	mLightColor = float4(1, 1, 1, 1);

	mMainLight = mCurrentScene->newEntity();

	mMainLight->addComponent<TransformComponent>(0, 0, float3(100, 1, 100));
	mMainLight->addComponent<PointLightComponent>(mLightPosition, 10, mLightColor.xyz, 10);
	mMainLight->addComponent<RendererComponent>(mMeshes.at("Cube"));

	mMainEntity = newModel("monkey_dancing.model");

	//mSkyBoxes["Blue Space"] = newSkyBox("bluespace_skybox.texture");
	//mSkyBoxes["Light Blue Space"] = newSkyBox("lightbluespace_skybox.texture");
	//mSkyBoxes["Red Space"] = newSkyBox("redspace_skybox.texture");

	for (auto& skybox : mSkyBoxes)
	{
		mSkyBoxNames.push_back(skybox.first.c_str());
	}

	//mMainSkyBox = mSkyBoxes["Blue Space"];

}

void TestApplication::onKeyUp(KeyUpEvent* e)
{
	Application::onKeyUp(e);

	switch (e->getKeyCode())
	{
	case kKeyCodeEscape:
		stopRendering();
		break;
	default:
		break;
	}
}

void TestApplication::onMouseMoved(MouseMovedEvent* e)
{
	Application::onMouseMoved(e);

	switch (e->getButton())
	{
	case kMouseButtonLeft:
		mCameraOrientation.x += e->getYRel() * 0.001f;
		mCameraOrientation.y += e->getXRel() * 0.001f;
	}

}

void TestApplication::onFrameStart(FrameEvent* e)
{
	Application::onFrameStart(e);

	mCurrentScene->setBackColor(mBackColor);

	mCurrentScene->getMainCamera().setPosition(mCameraPosition);
	mCurrentScene->getMainCamera().setOrientation(mCameraOrientation);

	mMainLight->getComponent<TransformComponent>()->position = mLightPosition;
	mMainLight->getComponent<PointLightComponent>()->color = mLightColor.xyz;

	mMainEntity->getComponent<SkeletalComponent>()->sate.setCyclic(true);
	mMainEntity->getComponent<SkeletalComponent>()->sate.addTime(e->getElapsedTime() * mAnimationSpeed);

	mMainEntity->getComponent<TransformComponent>()->position = mEntityPosition;
	mMainEntity->getComponent<TransformComponent>()->orientation = mEntityOrientation;
	mMainEntity->getComponent<TransformComponent>()->scaling = mEntityScaling;

	//mMainSkyBox->getComponent<TransformComponent>()->position = mCurrentScene->getMainCamera().getPosition();
}

void TestApplication::onFrameEnd(FrameEvent* e)
{
	Application::onFrameEnd(e);
}

void TestApplication::onImGui(FrameEvent* e)
{
	Application::onImGui(e);
	if (ImGui::Begin("World"))
	{
		ImGui::ColorPicker4("Background Color", mBackColor.v);

		static int temp = 0;
		if (ImGui::ListBox("Sky Box", &temp, mSkyBoxNames.data(), mSkyBoxNames.size()))
		{
			mMainSkyBox = mSkyBoxes.at(mSkyBoxNames[temp]);
		}

	}
	ImGui::End();

	if (ImGui::Begin("Camera"))
	{
		ImGui::DragFloat("X", &mCameraPosition.x);
		ImGui::DragFloat("Y", &mCameraPosition.y);
		ImGui::DragFloat("Z", &mCameraPosition.z);

		if (ImGui::Button("Reset Position"))
		{
			mCameraPosition = DEFAULT_CAMERA_POSITION;
		}

		ImGui::SliderAngle("Pitch", &mCameraOrientation.x);
		ImGui::SliderAngle("Yaw", &mCameraOrientation.y);
		ImGui::SliderAngle("Roll", &mCameraOrientation.z);

		if (ImGui::Button("Reset Orientation"))
		{
			mCameraOrientation = DEFAULT_CAMERA_ORIENTATION;
		}

	}
	ImGui::End();

	if (ImGui::Begin("Entity"))
	{
		ImGui::DragFloat("X", &mEntityPosition.x);
		ImGui::DragFloat("Y", &mEntityPosition.y);
		ImGui::DragFloat("Z", &mEntityPosition.z);

		if (ImGui::Button("Reset Position"))
		{
			mEntityPosition = DEFAULT_ENTITY_POSITION;
		}

		ImGui::SliderAngle("Pitch", &mEntityOrientation.x);
		ImGui::SliderAngle("Yaw", &mEntityOrientation.y);
		ImGui::SliderAngle("Roll", &mEntityOrientation.z);

		if (ImGui::Button("Reset Orientation"))
		{
			mEntityOrientation = DEFAULT_ENTITY_ORIENTATION;
		}

		ImGui::DragFloat("X Scaling", &mEntityScaling.x);
		ImGui::DragFloat("Y Scaling", &mEntityScaling.y);
		ImGui::DragFloat("Z Scaling", &mEntityScaling.z);

		if (ImGui::Button("Reset Scaling"))
		{
			mEntityScaling = DEFAULT_ENTITY_SCALING;
		}

		ImGui::SliderFloat("Animation Speed", &mAnimationSpeed, 0.0f, 2.0f);

	}
	ImGui::End();

	if (ImGui::Begin("Light"))
	{
		ImGui::DragFloat("X", &mLightPosition.x);
		ImGui::DragFloat("Y", &mLightPosition.y);
		ImGui::DragFloat("Z", &mLightPosition.z);

		if (ImGui::Button("Reset Position"))
		{
			mLightPosition = DEFAULT_LIGHT_POSITION;
		}

		ImGui::ColorPicker4("Color", mLightColor.v);
	}
	ImGui::End();
}
