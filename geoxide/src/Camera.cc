
#include "Geoxide/Camera.h"

namespace Geoxide {

	void Camera::updateViewMatrix(Renderer* gfx)
	{
		Matrix qua = NewMatrixQuaternion(mQuaternion);
		Vector center = NewVector(0, 0, 1);
		Vector up = NewVector(0, 1, 0);

		center = VectorMultiply(center, qua);
		up = VectorMultiply(up, qua);

		LookAtMatrixInput input;
		input.position = mPosition;
		input.center = VectorAdd(mPosition, center);
		input.up = up;

		gfx->makeLookAtMatrix(input, mViewMatrix);
	}

	void Camera::updateProjectionMatrix(Renderer* gfx)
	{
		switch (mProjectionType)
		{
		default:
		case kProjectionTypePerspective:
		{
			PerspectiveMatrixInput input;
			input.aspect = mWidth / mHeight;
			input.fov = mFov;
			input.nearZ = mNearZ;
			input.farZ = mFarZ;

			gfx->makePerspectiveMatrix(input, mProjectionMatrix);
			break;
		}
		case kProjectionTypeOrthographic:
		{
			OrthographicMatrixInput input;
			input.width = mWidth;
			input.height = mHeight;
			input.nearZ = mNearZ;
			input.farZ = mFarZ;

			gfx->makeOrthographicMatrix(input, mProjectionMatrix);
			break;
		}
		}
	}

}
