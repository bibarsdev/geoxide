
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
		MovableObject(app), mGfx(app->getRenderer()), mMesh(mesh) {}

	void MeshObject::update(Object* _parent)
	{
		MovableObject::update(_parent);

		DrawInput mDrawInput;

		mDrawInput.meshData = mMesh->getMeshData();
		mDrawInput.topology = kPrimitiveTopologyTriangleList;

		for (auto& subMesh : mMesh->getSubMeshes())
		{
			Material* material = subMesh.material;
			Matrix WorldViewProj = MatrixMultiply(getWorldMatrix(), mApp->getViewProjMatrix());

			WorldViewProj = MatrixTranspose(WorldViewProj);
				
			mDrawInput.vsUniformData = &WorldViewProj;
			mDrawInput.vsUniformDataSize = sizeof(WorldViewProj);

			mDrawInput.psUniformData = 0;
			mDrawInput.psUniformDataSize = 0;

			mDrawInput.program = material->getGpuProgram();

			mDrawInput.textures = material->getTextures().data();
			mDrawInput.numTextures = material->getTextures().size();

			mDrawInput.indexStart = subMesh.indexStart;
			mDrawInput.indexCount = subMesh.indexCount;

			mGfx->draw(mDrawInput);
		}
	}

}
