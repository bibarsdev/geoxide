
#ifndef __GX_MOVABLE_H__
#define __GX_MOVABLE_H__

namespace Geoxide {

	class Movable
	{
	public:
		Movable() : 
			mPosition(NewVector(0, 0, 0)), mQuaternion(NewVector(0, 0, 0, 1)), mScaling(NewVector(1, 1, 1)), mWorldMatrix(NewMatrixIdentity()) {}

		void setPosition(Vector v) { mPosition = v; }
		void setQuaternion(Vector v) { mQuaternion = v; }
		void setScaling(Vector v) { mScaling = v; }

		Vector getPosition() const { return mPosition; }
		Vector getRotation() const { return mQuaternion; }
		Vector getScaling() const { return mScaling; }

		MatrixConst getWorldMatrix() const { return mWorldMatrix; }

		void updateWorldMatrix() {
			Matrix positionMatrix = NewMatrixTranslation(mPosition);
			Matrix quaternionMatrix = NewMatrixQuaternion(mQuaternion);
			Matrix scalingMatrix = NewMatrixScaling(mScaling);

			mWorldMatrix = MatrixMultiply(positionMatrix, quaternionMatrix);
			mWorldMatrix = MatrixMultiply(mWorldMatrix, scalingMatrix);
		}

		void pitch(float rad)
		{
			mQuaternion = NewQuaternionAngleAxis(NewVector(1, 0, 0, rad));
		}

		void yaw(float rad)
		{
			mQuaternion = NewQuaternionAngleAxis(NewVector(0, 1, 0, rad));
		}

		void roll(float rad)
		{
			mQuaternion = NewQuaternionAngleAxis(NewVector(0, 0, 1, rad));
		}

	protected:
		Vector mPosition;
		Vector mQuaternion;
		Vector mScaling;
		Matrix mWorldMatrix;
	};

}

#endif // !__GX_MOVABLE_H__

