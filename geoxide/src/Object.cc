
#include "Geoxide/Object.h"

#include "Geoxide/Application.h"

namespace Geoxide {

	void Object::update(Object* parent)
	{
	}

	void MovableObject::update(Object* _parent)
	{
		Object::update(_parent);

		MovableObject* parent = (MovableObject*)_parent;

		updateWorldMatrix();
		mWorldMatrix = MatrixMultiply(mWorldMatrix, parent->getWorldMatrix());
	}

	MeshObject::MeshObject(Application* app, Mesh* mesh) :
		MovableObject(app), mGfx(app->getRenderer()), mMesh(mesh), 
		mVSData(app->getSharedBuffer()),
		mPSData(app->getSharedBuffer() + SHARED_BUFFER_SIZE / 2) {}

	void MeshObject::update(Object* _parent)
	{
		MovableObject::update(_parent);

		MatrixConst world = getWorldMatrix();
		Matrix worldViewProj = MatrixMultiply(world, mApp->getViewProjMatrix());
		float time = mApp->getTime();

		DrawInput drawInput;

		drawInput.meshData = mMesh->getMeshData();
		drawInput.topology = kPrimitiveTopologyTriangleList;

		for (auto& subMesh : mMesh->getSubMeshes())
		{
			Material* material = subMesh.material;

			uint32_t VSDataSize = material->getVSDataSize();
			uint32_t PSDataSize = material->getPSDataSize();

			uint32_t worldViewProjOffset = material->getWorldViewProjOffset();
			uint32_t worldOffset = material->getWorldOffset();
			uint32_t timeOffset = material->getTimeOffset();

			if (worldViewProjOffset != -1)
			{
				Matrix& worldViewProjUniform = (Matrix&)mVSData[worldViewProjOffset];
				worldViewProjUniform = worldViewProj;
			}

			if (worldOffset != -1)
			{
				Matrix& worldUniform = (Matrix&)mVSData[worldOffset];
				worldUniform = world;
			}

			if (timeOffset != -1)
			{
				float& timeUniform = (float&)mPSData[timeOffset];
				timeUniform = time;
			}

			drawInput.vsUniformData = mVSData;
			drawInput.vsUniformDataSize = VSDataSize;

			drawInput.psUniformData = mPSData;
			drawInput.psUniformDataSize = PSDataSize;

			drawInput.program = material->getGpuProgram();

			drawInput.textures = material->getTextures().data();
			drawInput.numTextures = material->getTextures().size();

			drawInput.indexStart = subMesh.indexStart;
			drawInput.indexCount = subMesh.indexCount;

			mGfx->draw(drawInput);
		}
	}

}
