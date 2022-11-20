
#ifndef __GX_OBJECT_H__
#define __GX_OBJECT_H__

#include "Renderer.h"

namespace Geoxide {

	class Object
	{
	public:
		using Buffer = std::vector<Object*>;
		using BufferIterator = Buffer::const_iterator;

	public:
		virtual ~Object() = default;
		virtual void update(Renderer* gfx, Object* parent) = 0;

		const Buffer& getChildren() const { return mChildren; }

		void addChild(Object* o) { mChildren.push_back(o); }

	protected:
		Buffer mChildren;
	};

	class MovableObject : public Object, public Movable
	{
	public:
		~MovableObject() = default;
		void update(Renderer* gfx, Object* parent) override {}
	};

	class MeshObject : public MovableObject
	{
	public:
		~MeshObject() = default;
		void update(Renderer* gfx, Object* _parent) override;

	protected:
		Mesh* mMesh;
	};

}

#endif // __GX_OBJECT_H__
