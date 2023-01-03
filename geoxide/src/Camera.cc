
#include "Geoxide/Camera.h"

#include <DirectXMath.h>

namespace Geoxide {

	void Camera::updateViewMatrix()
	{
		float4x4 rotationMatrix = float4x4::rotation_pitch_yaw_roll(mOrientation, rotationMatrix);

		float4 position = float4(mPosition.x, mPosition.y, mPosition.z, 1);
		float4 center = float4(0, 0, 1, 1);
		float4 up = float4(0, 1, 0, 1);

		center = rotationMatrix * center;
		up = rotationMatrix * up;

		center = position + center;

		//float4x4::look_at(mPosition, center, up, mViewMatrix);
		mViewMatrix = (float4x4&)DirectX::XMMatrixLookAtLH((DirectX::XMVECTOR&)position, (DirectX::XMVECTOR&)center, (DirectX::XMVECTOR&)up);
	}

	void Camera::updateProjectionMatrix(RendererDevice* gfx)
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
			//mProjectionMatrix = NewMatrixPerspective(mFov, mWidth / mHeight, mNearZ, mFarZ);
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
