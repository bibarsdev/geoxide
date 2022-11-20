
#include "Geoxide/Object.h"

namespace Geoxide {

	void MeshObject::update(Renderer* gfx, Object* _parent)
	{
		MeshObject* parent = (MeshObject*)_parent;

		Transform worldTransform;
		const Transform& parentTransform = parent->getTransform();

		worldTransform.position = VectorAdd(transform.position, parentTransform.position);
		worldTransform.quaternion = VectorMul(transform.quaternion, parentTransform.quaternion);
		worldTransform.scaling = VectorMul(transform.scaling, parentTransform.scaling);

		gfx->drawMesh(mMesh, worldTransform);
	}


}
