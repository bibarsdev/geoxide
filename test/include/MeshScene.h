
#ifndef __MESH_SCENE_H__
#define __MESH_SCENE_H__

#include "TestApplication.h"

class MeshScene : public Scene
{
public:
	MeshScene(TestApplication* app) : 
		Scene(app), mCubeMeshData(0), mColorProgram(0), mCubeMaterial(0), mCubeMesh(0), mCubeObject(0),
		mDebug0(0), mDebug1(0), mDebug2(0), mDebug3(0) {}
	~MeshScene() { destroyScene(); }

	void prepareScene() override;
	void destroyScene() override;

	void onFrameStart(Event*) override;
	void onFrameEnd(Event*) override;

protected:
	MeshData* mCubeMeshData;
	GpuProgram* mColorProgram;

	Material* mCubeMaterial;
	Mesh* mCubeMesh;
	MeshObject* mCubeObject;

	float mDebug0, mDebug1, mDebug2, mDebug3;
};

#endif
