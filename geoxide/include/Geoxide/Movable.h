
#ifndef __GX_MOVABLE_H__
#define __GX_MOVABLE_H__

namespace Geoxide {

	class Movable
	{
	public:
		void setPosition(cfloat3 v) { mPosition = v; }
		void setOrientation(cfloat3 v) { mOrientation = v; }
		void setScaling(cfloat3 v) { mScaling = v; }
		void setPitch(float rad) { mOrientation.x = rad; }
		void setYaw(float rad) { mOrientation.y = rad; }
		void setRoll(float rad) { mOrientation.z = rad; }

		cfloat3 getPosition() const { return mPosition; }
		cfloat3 getQuaternion() const { return mOrientation; }
		cfloat3 getScaling() const { return mScaling; }

	protected:
		Movable() :
			mScaling(1, 1, 1) {}

	protected:
		float3 mPosition;
		float3 mOrientation;
		float3 mScaling;
	};

}

#endif // !__GX_MOVABLE_H__

