
#include "Geoxide/Movable.h"

namespace Geoxide {

	void Movable::updateWorldMatrix()
	{
		Matrix positionMatrix = NewMatrixTranslation(mPosition);
		Matrix rotationMatrix = NewMatrixOrientation(mOrientation);
		Matrix scalingMatrix = NewMatrixScaling(mScaling);

		mWorldMatrix = MatrixMultiply(positionMatrix, rotationMatrix);
		mWorldMatrix = MatrixMultiply(mWorldMatrix, scalingMatrix);
	}

}
