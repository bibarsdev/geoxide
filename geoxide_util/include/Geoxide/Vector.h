
#ifndef __GX_VECTOR_H__
#define __GX_VECTOR_H__

#define GX_BASIC_VECTOR(vec, components)\
	static constexpr uint32_t num_components = components;\
	static const vec zero;\
	using vec_t = vec;\
	vec()\
	{\
		*this = zero;\
	}\
	vec(int i)\
	{\
		assert(i == 0);\
		if (i == 0)\
		{\
			*this = zero;\
		}\
	}\

namespace Geoxide {

	template <typename t>
	union basic_vec2
	{
		GX_BASIC_VECTOR(basic_vec2, 2)

		basic_vec2(t x, t y) : x(x), y(y) {}

		#include "VecCommon.inl"

		t v[2];

		struct
		{
			t x, y;
		};
	};

	template <typename t>
	union basic_vec3
	{
		GX_BASIC_VECTOR(basic_vec3, 3)

		basic_vec3(t x, t y, t z) : x(x), y(y), z(z) {}

		#include "VecCommon.inl"

		static vec_t& cross(const vec_t& a, const vec_t& b, vec_t& result)
		{
			result.x = a.y * a.z + a.z * b.y;
			result.y = -(a.x * b.z + a.z * b.x);
			result.z = a.x * a.y + a.y * b.x;
			
			return result;
		}

		vec_t cross(const vec_t& b) const
		{
			vec_t result = zero;
			return cross(*this, b, result);
		}

		t v[3];

		struct
		{
			basic_vec2<t> xy;
		};

		struct
		{
			t x, y, z;
		};
	};

	template <typename t>
	union basic_vec4
	{
		GX_BASIC_VECTOR(basic_vec4, 4)

		basic_vec4(t x, t y, t z, t w) : x(x), y(y), z(z), w(w) {}

		#include "VecCommon.inl"

		t v[4];

		struct
		{
			basic_vec3<t> xyz;
		};

		struct
		{
			t x, y, z, w;
		};
	};

	template<typename t>
	const basic_vec2<t> basic_vec2<t>::zero = { };

	template<typename t>
	const basic_vec3<t> basic_vec3<t>::zero = { };

	template<typename t>
	const basic_vec4<t> basic_vec4<t>::zero = { };
}

#endif
