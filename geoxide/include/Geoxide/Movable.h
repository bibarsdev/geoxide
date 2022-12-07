
#ifndef __GX_MOVABLE_H__
#define __GX_MOVABLE_H__

namespace Geoxide {

	class Movable
	{
	public:
		void setPosition(VectorConst v) { mPosition = v; }
		void setOrientation(VectorConst v) { mOrientation = v; }
		void setScaling(VectorConst v) { mScaling = v; }
		void setPitch(float rad) { VectorXRef(mOrientation) = rad; }
		void setYaw(float rad) { VectorYRef(mOrientation) = rad; }
		void setRoll(float rad) { VectorZRef(mOrientation) = rad; }

		VectorConst getPosition() const { return mPosition; }
		VectorConst getQuaternion() const { return mOrientation; }
		VectorConst getScaling() const { return mScaling; }

		MatrixConst getWorldMatrix() const { return mWorldMatrix; }

		void updateWorldMatrix();

	protected:
		Movable() :
			mPosition(NewVector(0, 0, 0)), mOrientation(NewVector(0, 0, 0)), mScaling(NewVector(1, 1, 1)), mWorldMatrix(NewMatrixIdentity()) {}

	protected:
		Vector mPosition;
		Vector mOrientation;
		Vector mScaling;
		Matrix mWorldMatrix;
	};

}

#endif // !__GX_MOVABLE_H__

