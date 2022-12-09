
#include "Geoxide/SceneManager.h"

namespace Geoxide {

	void Entity::update(SceneNode* parent)
	{
	}

	void MovableEntity::update(SceneNode* node)
	{
		Entity::update(node);

		updateWorldMatrix();

		SceneNode* parentNode = node->getParent();

		if (parentNode)
		{
			MovableEntity* parent = (MovableEntity*)parentNode->getEntity();

			if (parent)
				mWorldMatrix = MatrixMultiply(mWorldMatrix, parent->getWorldMatrix());
		}

	}

	void AnimableEntity::update(SceneNode* node)
	{
		MovableEntity::update(node);
	}

	void ModelEntity::update(SceneNode* node)
	{
		AnimableEntity::update(node);

		SceneManager* scnMan = node->getSceneManager();
		Renderer* gfx = scnMan->getRenderer();
		const SubMesh* subMeshes = mMesh->getSubMeshes();
		uint32_t numSubMeshes = mMesh->getNumSubMeshes();

		scnMan->updateSceneBuffer(mWorldMatrix);
		scnMan->updateLightBuffer(mWorldMatrix);

		if (mAnimationState.hasAnimation())
			scnMan->updateSkeletonBuffer(mAnimationState.getCurrentFrame());

		GpuBuffer* sceneBuffer = scnMan->getSceneBuffer();
		GpuBuffer* skeletonBuffer = scnMan->getSkeletonBuffer();
		GpuBuffer* lightBuffer = scnMan->getLightBuffer();
		GpuBuffer* materialBuffer = scnMan->getMaterialBuffer();

		GpuBuffer* VSBuffers[3];
		GpuBuffer* PSBuffers[3];

		DrawInput drawInput;

		drawInput.meshData = mMesh->getMeshData();
		drawInput.topology = kPrimitiveTopologyTriangleList;

		for (uint32_t i = 0; i < numSubMeshes; i++)
		{
			const SubMesh& subMesh = subMeshes[i];

			Material* material = subMesh.material;

			scnMan->updateMaterialBuffer(material->getData(), material->getDataSize());

			uint32_t numVSBuffers = 0;
			uint32_t numPSBuffers = 0;
			
			uint32_t sceneBufferIndex = material->getSceneBufferIndex();
			uint32_t lightBufferIndex = material->getLightBufferIndex();
			uint32_t materialBufferIndex = material->getMaterialBufferIndex();
			uint32_t skeletonBufferIndex = material->getSkeletonBufferIndex();

			if (sceneBufferIndex != -1)
			{
				VSBuffers[sceneBufferIndex] = sceneBuffer;
				numVSBuffers++;
			}

			if (skeletonBufferIndex != -1)
			{
				VSBuffers[skeletonBufferIndex] = skeletonBuffer;
				numVSBuffers++;
			}

			if (lightBufferIndex != -1)
			{
				PSBuffers[lightBufferIndex] = lightBuffer;
				numPSBuffers++;
			}

			if (materialBufferIndex != -1)
			{
				PSBuffers[materialBufferIndex] = materialBuffer;
				numPSBuffers++;
			}

			drawInput.VSBuffers = VSBuffers;
			drawInput.PSBuffers = PSBuffers;

			drawInput.numVSBuffers = numVSBuffers;
			drawInput.numPSBuffers = numPSBuffers;

			drawInput.state = material->getRenderState();

			drawInput.program = material->getGpuProgram();

			drawInput.textures = material->getTextures();
			drawInput.numTextures = material->getNumTextures();

			drawInput.indexStart = subMesh.indexStart;
			drawInput.indexCount = subMesh.indexCount;

			gfx->draw(drawInput);
		}
	}
}
