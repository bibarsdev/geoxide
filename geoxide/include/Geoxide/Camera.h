
#ifndef __GX_CAMERA_H__
#define __GX_CAMERA_H__

#include "Movable.h"
#include "Geoxide/RendererEnum.h"

namespace Geoxide {

	class Camera : public Movable
	{
	public:
		Camera() :
			width(0), height(0), nearZ(0), farZ(0), fov(0), projectionType(kProjectionTypeUnknown) {}
		Camera(float width, float height, float nearZ, float farZ, float fov, ProjectionType projectionType) :
			width(width), height(height), nearZ(nearZ), farZ(farZ), fov(fov), projectionType(projectionType) {}

		void setProjectionType(ProjectionType type) { projectionType = type; }
		void setNearZ(float z) { nearZ = z; }
		void setFarZ(float z) { farZ = z; }
		void setFOV(float rad) { fov = rad; }
		void setViewportWidth(float w) { width = w; }
		void setViewportHeight(float h) { height = h; }
		void setBackGroundColor(ColorRGBA c) { backColor = c; }

		ProjectionType getProjectionType() const { return projectionType; }
		float getNearZ() const { return nearZ; }
		float getFarZ() const { return farZ; }
		float getFOV() const { return fov; }
		float getViewportWidth() const { return width; }
		float getViewportHeight() const { return height; }
		ColorRGBA getBackGroundColor() const { return backColor; }

	private:
		float width, height;
		float nearZ, farZ;
		float fov;
		ColorRGBA backColor;
		ProjectionType projectionType;
	};

}

#endif // !__GX_MATERIAL_H__

