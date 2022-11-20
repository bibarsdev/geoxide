
#include "Geoxide/Math3D.h"

namespace Geoxide {
namespace Math3D {

	Matrix NewIdentityMatrix()
	{
#ifdef SSE_INST
		Matrix m = {};
		m.r[0].m128_f32[0] = 1;
		m.r[1].m128_f32[1] = 1;
		m.r[2].m128_f32[2] = 1;
		m.r[3].m128_f32[3] = 1;
		return m;
#endif
	}

	Matrix MatrixMultiply(Matrix& m1, Matrix& m2)
	{
		Matrix result;
#ifdef SSE_INST
		Vector vW = m1.r[0];
		Vector vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		Vector vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		Vector vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, m2.r[0]);
		vY = _mm_mul_ps(vY, m2.r[1]);
		vZ = _mm_mul_ps(vZ, m2.r[2]);
		vW = _mm_mul_ps(vW, m2.r[3]);

		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[0] = vX;

		vW = m1.r[1];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, m2.r[0]);
		vY = _mm_mul_ps(vY, m2.r[1]);
		vZ = _mm_mul_ps(vZ, m2.r[2]);
		vW = _mm_mul_ps(vW, m2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[1] = vX;

		vW = m1.r[2];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, m2.r[0]);
		vY = _mm_mul_ps(vY, m2.r[1]);
		vZ = _mm_mul_ps(vZ, m2.r[2]);
		vW = _mm_mul_ps(vW, m2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[2] = vX;

		vW = m1.r[3];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, m2.r[0]);
		vY = _mm_mul_ps(vY, m2.r[1]);
		vZ = _mm_mul_ps(vZ, m2.r[2]);
		vW = _mm_mul_ps(vW, m2.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[3] = vX;
		return result;
#endif
	}

	Matrix NewMatrixTranslation(Vector vec)
	{
#ifdef SSE_INST
		Matrix m = {};
		m.r[0].m128_f32[0] = 1;
		m.r[1].m128_f32[1] = 1;
		m.r[2].m128_f32[2] = 1;
		m.r[3] = vec;
		return m;
#endif
	}

}
}
