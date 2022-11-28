
#ifndef __GX_OBJECT_H__
#define __GX_OBJECT_H__

#include "Renderer.h"
#include "Movable.h"
#include "Mesh.h"

namespace Geoxide {

	class Application;

	class Object
	{
	public:
		using Buffer = std::vector<Object*>;
		using BufferIterator = Buffer::const_iterator;

	public:
		Object(Application* app) : mApp(app) {}
		virtual ~Object() = default;

		Application* getApplication() const { return mApp; }
		const Buffer& getChildren() const { return mChildren; }

		virtual void update(Object* parent);

		void addChild(Object* o) { mChildren.push_back(o); }
		void clearChildren() { mChildren.clear(); }

	protected:
		Application* mApp;
		Buffer mChildren;
	};

	class MovableObject : public Object, public Movable
	{
	public:
		MovableObject(Application* app) : Object(app) {}
		virtual ~MovableObject() = default;

		virtual void update(Object* parent) override;
	};

	class MeshObject : public MovableObject
	{
	public:
		MeshObject(Application* app, Mesh* mesh);
		virtual ~MeshObject() = default;

		Renderer* getRenderer() { return mGfx; };
		Mesh* getMesh() { return mMesh; };

		virtual void update(Object* parent) override;

	protected:
		Renderer* mGfx;
		uint8_t* mVSData;
		uint8_t* mPSData;
		Mesh* mMesh;
	};
}

#endif // __GX_OBJECT_H__
