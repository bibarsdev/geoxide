
#ifndef __GX_MATH_3D_H__
#define __GX_MATH_3D_H__

#include <immintrin.h>

namespace Geoxide {

#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64)
#define SSE_INST
	typedef __m128 ColorRGBA, Vector;
	typedef struct { __m128 r[4]; } Matrix;
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || __arm__ || __aarch64__
#define NEON_INST
// TODO
#endif

	struct Transform
	{
		Vector position;
		Vector quaternion;
		Vector scaling;
	};

	ColorRGBA NewColor(float r, float g, float b, float a = 1);
	float Red(ColorRGBA color);
	float Blue(ColorRGBA color);
	float Green(ColorRGBA color);
	float Alpha(ColorRGBA color);

	Vector NewVector(float x, float y, float z, float w = 1);
	Vector VectorAdd(Vector v1, Vector v2);
	Vector VectorMul(Vector v1, Vector v2);

	Matrix NewIdentityMatrix();
	Matrix MatrixMultiply(Matrix& m1, Matrix& m2);
	Matrix NewMatrixTranslation(Vector vec);

}

#endif
