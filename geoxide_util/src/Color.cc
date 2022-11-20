
#include "Geoxide/Math3D.h"

namespace Geoxide {
namespace Math3D {

	ColorRGBA NewColor(float r, float g, float b, float a)
	{
#ifdef SSE_INST
		return _mm_set_ps(a, b, g, r);
#endif
	}

	float Red(ColorRGBA color)
	{
#ifdef SSE_INST
		return color.m128_f32[0];
#endif
	}

	float Blue(ColorRGBA color)
	{
#ifdef SSE_INST
		return color.m128_f32[1];
#endif
	}

	float Green(ColorRGBA color)
	{
#ifdef SSE_INST
		return color.m128_f32[2];
#endif
	}

	float Alpha(ColorRGBA color)
	{
#ifdef SSE_INST
		return color.m128_f32[3];
#endif
	}

}
}
