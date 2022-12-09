
#ifndef __GX_ENTITY_H__
#define __GX_ENTITY_H__

#include "Renderer.h"
#include "Movable.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"

namespace Geoxide {

	class SceneNode;

	class Entity
	{
	public:
		virtual ~Entity() = default;
		virtual void update(SceneNode* node);
	};

	class MovableEntity : public Entity, public Movable
	{
	public:
		virtual ~MovableEntity() = default;
		virtual void update(SceneNode* node) override;
	};

	class AnimableEntity : public MovableEntity
	{
	public:
		AnimableEntity() {}

		virtual ~AnimableEntity() = default;
		virtual void update(SceneNode* node) override;

		SkeletalAnimationState& getAnimationState() { return mAnimationState; };

	protected:
		SkeletalAnimationState mAnimationState;
	};

	class ModelEntity : public AnimableEntity
	{
	public:
		ModelEntity() : mMesh(0), mSkeleton(0) {}
		ModelEntity(Mesh* mesh, Skeleton* skeleton) : mMesh(mesh), mSkeleton(skeleton) {}

		virtual ~ModelEntity() = default;
		virtual void update(SceneNode* node) override;

		Mesh* getMesh() { return mMesh; };

	protected:
		Mesh* mMesh;
		Skeleton* mSkeleton;
	};

}

#endif // __GX_ENTITY_H__
