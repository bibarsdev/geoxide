
#ifndef __GX_MATH_3D_H__
#define __GX_MATH_3D_H__

namespace Geoxide {

	// TODO: SIMD

	struct Vector
	{
		float x, y, z, w;
	};

	struct Matrix
	{
		Vector r[4];
	};

	// For convienency later when we add SIMD
	using VectorConst = const Vector&;
	using MatrixConst = const Matrix&;

	inline Vector NewVector(float x, float y, float z, float w = 0)
	{
		return Vector{ x,y,z,w };
	}

	inline float VectorX(VectorConst v) { return v.x; }
	inline float VectorY(VectorConst v) { return v.y; }
	inline float VectorZ(VectorConst v) { return v.z; }
	inline float VectorW(VectorConst v) { return v.w; }

	inline Vector VectorAdd(VectorConst v1, VectorConst v2)
	{
		Vector result;

		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w + v2.w;

		return result;
	}

	inline Vector VectorMultiply(VectorConst v, MatrixConst m)
	{
		Vector result = {};

		result.x += v.x * m.r[0].x;
		result.x += v.y * m.r[1].x;
		result.x += v.z * m.r[2].x;
		result.x += v.w * m.r[3].x;

		result.y += v.x * m.r[0].y;
		result.y += v.y * m.r[1].y;
		result.y += v.z * m.r[2].y;
		result.y += v.w * m.r[3].y;

		result.z += v.x * m.r[0].z;
		result.z += v.y * m.r[1].z;
		result.z += v.z * m.r[2].z;
		result.z += v.w * m.r[3].z;

		result.w += v.x * m.r[0].w;
		result.w += v.y * m.r[1].w;
		result.w += v.z * m.r[2].w;
		result.w += v.w * m.r[3].w;

		return result;
	}

	inline float VectorDot(VectorConst v1, VectorConst v2)
	{
		float result;

		result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;

		return result;
	}

	inline Vector VectorCross(VectorConst v1, VectorConst v2)
	{
		Vector result;

		result.x = v1.y * v2.z + v1.z * v2.y;
		result.y = -(v1.x * v2.z + v1.z * v2.x);
		result.z = v1.x * v2.y + v1.y * v2.x;
		result.w = 0;

		return result;
	}

	inline float VectorLength(VectorConst v)
	{
		float result;

		result = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

		return result;
	}

	inline float VectorAngleBetween(VectorConst v1, VectorConst v2)
	{
		float result;

		float dot = VectorDot(v1, v2);
		float v1Length = VectorLength(v1);
		float v2Length = VectorLength(v2);

		result = acos(dot / (v1Length * v2Length));

		return result;
	}

	inline Vector VectorNormalize(VectorConst v)
	{
		Vector result = v;

		float length = VectorLength(result);

		result.x /= length;
		result.y /= length;
		result.z /= length;
		result.w /= length;

		return result;
	}

	inline Vector NewQuaternionIdentity()
	{
		return Vector{ 0,0,0,1 };
	}

	inline Vector NewQuaternionAngleAxis(VectorConst v)
	{
		Vector result;

		float angle = VectorW(v);
		float halfAngle = angle / 2;
		float halfAngleSin = sin(halfAngle);

		result.x = v.x * halfAngleSin;
		result.y = v.y * halfAngleSin;
		result.z = v.z * halfAngleSin;
		result.w = cos(halfAngle);

		return result;
	}

	inline Vector QuaternionConjugate(VectorConst v)
	{
		Vector result;

		result.x = -v.x;
		result.y = -v.y;
		result.z = -v.z;
		result.w = v.w;

		return result;
	}

	const Vector kIdentityRow0 = NewVector(1, 0, 0, 0);
	const Vector kIdentityRow1 = NewVector(0, 1, 0, 0);
	const Vector kIdentityRow2 = NewVector(0, 0, 1, 0);
	const Vector kIdentityRow3 = NewVector(0, 0, 0, 1);

	inline Matrix NewMatrixIdentity()
	{
		Matrix result;

		result.r[0] = kIdentityRow0;
		result.r[1] = kIdentityRow1;
		result.r[2] = kIdentityRow2;
		result.r[3] = kIdentityRow3;

		return result;
	}

	inline Matrix NewMatrixTranslation(VectorConst v)
	{
		Matrix result;

		result.r[0] = kIdentityRow0;
		result.r[1] = kIdentityRow1;
		result.r[2] = kIdentityRow2;
		result.r[3] = { VectorX(v), VectorY(v), VectorZ(v), 1 };

		return result;
	}

	inline Matrix NewMatrixQuaternion(VectorConst v)
	{
		Matrix result;

		float x = VectorX(v), y = VectorY(v), z = VectorZ(v), w = VectorW(v);
		float sx = x * x, sy = y * y, sz = z * z;

		result.r[0].x = 1.f - 2 * (sy + sz);
		result.r[0].y = 2 * (x * y - w * z);
		result.r[0].z = 2 * (x * z + w * y);
		result.r[0].w = 0;

		result.r[1].x = 2 * (x * y + w * z);
		result.r[1].y = 1.f - 2 * (sx + sz);
		result.r[1].z = 2 * (y * z - w * x);
		result.r[1].w = 0;

		result.r[2].x = 2 * (x * z - w * y);
		result.r[2].y = 2 * (y * z + w * x);
		result.r[2].z = 1.f - 2 * (sx + sy);
		result.r[2].w = 0;

		result.r[3] = kIdentityRow3;

		return result;
	}

	inline Matrix NewMatrixScaling(VectorConst v)
	{
		Matrix result;

		result.r[0] = { VectorX(v),0,0,0 };
		result.r[1] = { 0,VectorY(v),0,0 };
		result.r[2] = { 0,0,VectorZ(v),0 };
		result.r[3] = kIdentityRow3;

		return result;
	}

	inline Matrix MatrixMultiply(MatrixConst m1, MatrixConst m)
	{
		Matrix result = {};

		for (uint32_t i = 0; i < 4; ++i)
		{
			result.r[i].x += m1.r[i].x * m.r[0].x;
			result.r[i].x += m1.r[i].y * m.r[1].x;
			result.r[i].x += m1.r[i].z * m.r[2].x;
			result.r[i].x += m1.r[i].w * m.r[3].x;

			result.r[i].y += m1.r[i].x * m.r[0].y;
			result.r[i].y += m1.r[i].y * m.r[1].y;
			result.r[i].y += m1.r[i].z * m.r[2].y;
			result.r[i].y += m1.r[i].w * m.r[3].y;

			result.r[i].z += m1.r[i].x * m.r[0].z;
			result.r[i].z += m1.r[i].y * m.r[1].z;
			result.r[i].z += m1.r[i].z * m.r[2].z;
			result.r[i].z += m1.r[i].w * m.r[3].z;

			result.r[i].w += m1.r[i].x * m.r[0].w;
			result.r[i].w += m1.r[i].y * m.r[1].w;
			result.r[i].w += m1.r[i].z * m.r[2].w;
			result.r[i].w += m1.r[i].w * m.r[3].w;
		}

		return result;
	}

	inline Matrix MatrixTranspose(MatrixConst m)
	{
		Matrix result;

		result.r[0].x = m.r[0].x;
		result.r[1].x = m.r[0].y;
		result.r[2].x = m.r[0].z;
		result.r[3].x = m.r[0].w;

		result.r[0].y = m.r[1].x;
		result.r[1].y = m.r[1].y;
		result.r[2].y = m.r[1].z;
		result.r[3].y = m.r[1].w;

		result.r[0].z = m.r[2].x;
		result.r[1].z = m.r[2].y;
		result.r[2].z = m.r[2].z;
		result.r[3].z = m.r[2].w;

		result.r[0].w = m.r[3].x;
		result.r[1].w = m.r[3].y;
		result.r[2].w = m.r[3].z;
		result.r[3].w = m.r[3].w;

		return result;
	}

}

#endif
