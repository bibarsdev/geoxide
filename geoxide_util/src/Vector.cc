
#include "Geoxide/Math3D.h"

namespace Geoxide {

	Vector NewVector(float x, float y, float z, float w)
	{
#ifdef SSE_INST
		return _mm_set_ps(w, z, y, x);
#endif
	}

	Vector VectorAdd(Vector v1, Vector v2)
	{
#ifdef SSE_INST
		return _mm_add_ps(v1, v2);
#endif
	}

	Vector VectorMul(Vector v1, Vector v2)
	{
#ifdef SSE_INST
		return _mm_mul_ps(v1, v2);
#endif
	}

}
