
#ifndef __GX_CAMERA_H__
#define __GX_CAMERA_H__

#include "Movable.h"
#include "Renderer.h"

namespace Geoxide {

	class Camera : public Movable
	{
	public:
		enum ProjectionType
		{
			kProjectionTypeUnknown,
			kProjectionTypePerspective,
			kProjectionTypeOrthographic,
		};

	public:
		Camera() :
			mWidth(0), mHeight(0), mNearZ(0), mFarZ(0), mFov(0), mProjectionType(kProjectionTypeUnknown) {}
		Camera(float width, float height, float nearZ, float farZ, float fov, ProjectionType projectionType) :
			mWidth(width), mHeight(height), mNearZ(nearZ), mFarZ(farZ), mFov(fov), mProjectionType(projectionType) {}

		void setProjectionType(ProjectionType type) { mProjectionType = type; }
		void setNearZ(float z) { mNearZ = z; }
		void setFarZ(float z) { mFarZ = z; }
		void setFOV(float rad) { mFov = rad; }
		void setViewWidth(float w) { mWidth = w; }
		void setViewHeight(float h) { mHeight = h; }

		ProjectionType getProjectionType() const { return mProjectionType; }
		float getNearZ() const { return mNearZ; }
		float getFarZ() const { return mFarZ; }
		float getFOV() const { return mFov; }
		float getViewWidth() const { return mWidth; }
		float getViewHeight() const { return mHeight; }

		MatrixConst getViewMatrix() { return mViewMatrix; }
		MatrixConst getProjectionMatrix() { return mProjectionMatrix; }

		void updateViewMatrix(Renderer* gfx);
		void updateProjectionMatrix(Renderer* gfx);

	private:
		float mWidth, mHeight;
		float mNearZ, mFarZ;
		float mFov;
		ProjectionType mProjectionType;
		Matrix mViewMatrix;
		Matrix mProjectionMatrix;
	};

}

#endif // !__GX_MATERIAL_H__

