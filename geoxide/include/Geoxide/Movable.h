
#ifndef __GX_MOVABLE_H__
#define __GX_MOVABLE_H__

namespace Geoxide {

	class Movable
	{
	public:
		void setPosition(Vector v) { transform.position = v; }
		void setRotation(Vector v) { transform.quaternion = v; }
		void setScaling(Vector v) { transform.scaling = v; }

		const Vector getPosition() const { return transform.position; }
		const Vector getRotation() const { return transform.quaternion; }
		const Vector getScaling() const { return transform.scaling; }

		const Transform& getTransform() const { return transform; }

	protected:
		Transform transform;
	};

}

#endif // !__GX_MOVABLE_H__

