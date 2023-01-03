
#ifndef __GX_MATH_TYPES_H__
#define __GX_MATH_TYPES_H__

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace Geoxide {

	using byte2 = basic_vec2<unsigned char>;
	using byte3 = basic_vec3<unsigned char>;
	using byte4 = basic_vec4<unsigned char>;

	using int2 = basic_vec2<int>;
	using int3 = basic_vec3<int>;
	using int4 = basic_vec4<int>;

	using uint2 = basic_vec2<unsigned int>;
	using uint3 = basic_vec3<unsigned int>;
	using uint4 = basic_vec4<unsigned int>;

	using float2 = basic_vec2<float>;
	using float3 = basic_vec3<float>;
	using float4 = basic_vec4<float>;

	using double2 = basic_vec2<double>;
	using double3 = basic_vec3<double>;
	using double4 = basic_vec4<double>;

	using byte2x2 = basic_matrix2x2<unsigned char>;
	using byte3x3 = basic_matrix3x3<unsigned char>;
	using byte4x4 = basic_matrix4x4<unsigned char>;

	using int2x2 = basic_matrix2x2<int>;
	using int3x3 = basic_matrix3x3<int>;
	using int4x4 = basic_matrix4x4<int>;

	using uint2x2 = basic_matrix2x2<unsigned int>;
	using uint3x3 = basic_matrix3x3<unsigned int>;
	using uint4x4 = basic_matrix4x4<unsigned int>;

	using float2x2 = basic_matrix2x2<float>;
	using float3x3 = basic_matrix3x3<float>;
	using float4x4 = basic_matrix4x4<float>;

	using double2x2 = basic_matrix2x2<double>;
	using double3x3 = basic_matrix3x3<double>;
	using double4x4 = basic_matrix4x4<double>;

	using cint2 = const int2&;
	using cint3 = const int3&;
	using cint4 = const int4&;

	using cuint2 = const uint2&;
	using cuint3 = const uint3&;
	using cuint4 = const uint4&;

	using cfloat2 = const float2&;
	using cfloat3 = const float3&;
	using cfloat4 = const float4&;

	using cint2x2 = const int2x2&;
	using cint3x3 = const int3x3&;
	using cint4x4 = const int4x4&;

	using cuint2x2 = const uint2x2&;
	using cuint3x3 = const uint3x3&;
	using cuint4x4 = const uint4x4&;

	using cfloat2x2 = const float2x2&;
	using cfloat3x3 = const float3x3&;
	using cfloat4x4 = const float4x4&;

	using quat = basic_quat<float>;
	using quatd = basic_quat<double>;

	using cquat = const quat&;
	using cquatd = const quatd&;
}

#endif
