
#include "Geoxide/Movable.h"

namespace Geoxide {

	void Movable::updateWorldMatrix() {
		Matrix positionMatrix = NewMatrixTranslation(mPosition);
		Matrix quaternionMatrix = NewMatrixQuaternion(mQuaternion);
		Matrix scalingMatrix = NewMatrixScaling(mScaling);

		mWorldMatrix = MatrixMultiply(positionMatrix, quaternionMatrix);
		mWorldMatrix = MatrixMultiply(mWorldMatrix, scalingMatrix);
	}

}
