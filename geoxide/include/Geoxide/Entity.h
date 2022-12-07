
#ifndef __GX_ENTITY_H__
#define __GX_ENTITY_H__

#include "Renderer.h"
#include "Movable.h"
#include "Mesh.h"

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

	class MeshEntity : public MovableEntity
	{
	public:
		MeshEntity(Mesh* mesh) : mMesh(mesh) {}
		virtual ~MeshEntity() = default;
		virtual void update(SceneNode* node) override;

		Mesh* getMesh() { return mMesh; };

	protected:
		Mesh* mMesh;
	};

}

#endif // __GX_ENTITY_H__
