
#ifndef __GX_MATRIX_H__
#define __GX_MATRIX_H__

#include "Vector.h"
#include "Quaternion.h"

#define GX_BASIC_MATRIX(matrix, vec, rows, columns)\
	static constexpr uint32_t num_rows = rows;\
	static constexpr uint32_t num_columns = columns;\
	static const matrix zero;\
	using vec_t = vec<t>;\
	using matrix_t = matrix;\
	matrix()\
	{\
	}\
	matrix(t v)\
	{\
		if (v == (t)1.f)\
		{\
			*this = identity;\
		}\
		else\
		{\
			*this = zero;\
			for (size_t i = 0; i < num_rows; i++)\
			{\
				m[i][i] = v; \
			}\
		}\
	}\

namespace Geoxide {

	template <typename t>
	union basic_matrix2x2
	{
		GX_BASIC_MATRIX(basic_matrix2x2, basic_vec2, 2, 2)

		static const matrix_t identity;

		basic_matrix2x2(const vec_t& r0, const vec_t& r1)
			: r0(r0), r1(r1) {}

		#include "MatrixCommon.inl"

		t m[2][2];
		vec_t r[2];

		struct
		{
			vec_t r0;
			vec_t r1;
		};
	};

	template <typename t>
	union basic_matrix3x3
	{
		GX_BASIC_MATRIX(basic_matrix3x3, basic_vec3, 3, 3)

		static const matrix_t identity;

		basic_matrix3x3(const vec_t& r0, const vec_t& r1, const vec_t& r2)
			: r0(r0), r1(r1), r2(r2) {}

		#include "MatrixCommon.inl"

		t m[3][3];
		vec_t r[3];

		struct
		{
			vec_t r0;
			vec_t r1;
			vec_t r2;
		};
	};

	template <typename t>
	union basic_matrix4x4
	{
		GX_BASIC_MATRIX(basic_matrix4x4, basic_vec4, 4, 4)

		static const matrix_t identity;

		basic_matrix4x4(const vec_t& r0, const vec_t& r1, const vec_t& r2, const vec_t& r3)
			: r0(r0), r1(r1), r2(r2), r3(r3) {}

		#include "MatrixCommon.inl"

		static matrix_t& look_at(const basic_vec3<t>& position, const basic_vec3<t>& center, const basic_vec3<t>& up, matrix_t& result)
		{
			result = identity;

			basic_vec3<t> forward = position - center;
			forward.normalize();

			basic_vec3<t> right = up.cross(forward);
			right.normalize();

			basic_vec3<t> up1 = forward.cross(right);

			result[0].xyz = right;
			result[1].xyz = up1;
			result[2].xyz = forward;
			result[3].xyz = position;

			return result;
		}

		static matrix_t& perspective(t fov, t aspect, t nearZ, t farZ, matrix_t& result)
		{
			result = zero;

			t tanHFov = tan(fov / 2.f);
			t farNearSum = farZ + nearZ;
			t NearfarDif = nearZ - farZ;

			result[0][0] = 1.f / (aspect * tanHFov);
			result[1][1] = 1.f / tanHFov;
			result[2][2] = farNearSum / NearfarDif;
			result[2][3] = (2 * farZ * nearZ) / NearfarDif;
			result[3][2] = 1;

			return result;
		}

		static matrix_t& translation(t x, t y, t z, matrix_t& result)
		{
			result = identity;

			result[0][3] = x;
			result[1][3] = y;
			result[2][3] = z;

			return result;
		}

		static matrix_t& translation(const basic_vec3<t>& v, matrix_t& result)
		{
			return translation(v.x, v.y, v.z, result);
		}

		static matrix_t& rotation_quaternion(const basic_quat<t>& v, matrix_t& result)
		{
			result = identity;

			float x = v.x, y = v.y, z = v.z, w = v.w;
			/*float xx = x * x, yy = y * y, zz = z * z;

			result.r[0][0] = 1.f - 2 * (yy + zz);
			result.r[0][1] = 2 * (x * y - w * z);
			result.r[0][2] = 2 * (x * z + w * y);

			result.r[1][2] = 2 * (x * y + w * z);
			result.r[1][1] = 1.f - 2 * (xx + zz);
			result.r[1][2] = 2 * (y * z - w * x);

			result.r[2][0] = 2 * (x * z - w * y);
			result.r[2][1] = 2 * (y * z + w * x);
			result.r[2][2] = 1.f - 2 * (xx + yy);*/

			result[0][0] = 1.f - 2.f * (y * y + z * z);
			result[0][1] = 2.f * (x * y - z * w);
			result[0][2] = 2.f * (x * z + y * w);

			result[1][0] = 2.f * (x * y + z * w);
			result[1][1] = 1.f - 2.f * (x * x + z * z);
			result[1][2] = 2.f * (y * z - x * w);

			result[2][0] = 2.f * (x * z - y * w);
			result[2][1] = 2.f * (y * z + x * w);
			result[2][2] = 1.f - 2.f * (x * x + y * y);

			return result;
		}

		static matrix_t& rotation_axis(const basic_vec3<t>& axis, t rad, matrix_t& result)
		{
			result = identity;

			t cos_rad = cos(rad);
			t sin_rad = sin(rad);

			t cosx = cos_rad * axis.x;
			t cosy = cos_rad * axis.y;
			t cosz = cos_rad * axis.z;

			t sinx = sin_rad * axis.x;
			t siny = sin_rad * axis.y;
			t sinz = sin_rad * axis.z;

			result[0][0] = cosy * cosz;
			result[0][1] = -cosy * sinz;
			result[0][2] = siny;

			result[1][0] = cosx * sinz + sinx * siny * cosz;
			result[1][1] = cosx * cosz - sinx * siny * sinz;
			result[1][2] = -sinx * cosy;

			result[1][0] = sinx * sinz - cosx * siny * cosz;
			result[1][1] = sinx * cosz + cosx * siny * sinz;
			result[1][2] = cosx * cosy;

			return result;
		}

		static matrix_t& rotation_pitch(t rad, matrix_t& result)
		{
			basic_quat<t> temp = basic_quat<t>::axis(basic_vec3<t>(1, 0, 0), rad, temp);
			return rotation_quaternion(temp, result);
		}

		static matrix_t& rotation_yaw(t rad, matrix_t& result)
		{
			basic_quat<t> temp = basic_quat<t>::axis(basic_vec3<t>(0, 1, 0), rad, temp);
			return rotation_quaternion(temp, result);
		}

		static matrix_t& rotation_roll(t rad, matrix_t& result)
		{
			basic_quat<t> temp = basic_quat<t>::axis(basic_vec3<t>(0, 0, 1), rad, temp);
			return rotation_quaternion(temp, result);
		}

		static matrix_t& rotation_pitch_yaw_roll(t pitch, t yaw, t roll, matrix_t& result)
		{
			matrix_t pitch_mat = rotation_pitch(pitch, pitch_mat);
			matrix_t yaw_mat = rotation_yaw(yaw, yaw_mat);
			matrix_t roll_mat = rotation_roll(roll, roll_mat);

			result = pitch_mat * yaw_mat * roll_mat;

			return result;
		}

		static matrix_t& rotation_pitch_yaw_roll(const basic_vec3<t>& pitch_yaw_roll, matrix_t& result)
		{
			return rotation_pitch_yaw_roll(pitch_yaw_roll.x, pitch_yaw_roll.y, pitch_yaw_roll.z, result);
		}

		static matrix_t& scaling(t x, t y, t z, matrix_t& result)
		{
			result = identity;

			result[0][0] = x;
			result[1][1] = y;
			result[2][2] = z;

			return result;
		}

		static matrix_t& scaling(const basic_vec3<t>& v, matrix_t& result)
		{
			return scaling(v.x, v.y, v.z, result);
		}

		basic_matrix3x3<t> get3x3() const
		{
			return basic_matrix3x3<t>((basic_vec3<t>&)r[0], (basic_vec3<t>&)r[1], (basic_vec3<t>&)r[2]);
		}

		t m[4][4];
		vec_t r[4];

		struct
		{
			vec_t r0;
			vec_t r1;
			vec_t r2;
			vec_t r3;
		};
	};

	template<typename t>
	const basic_matrix2x2<t> basic_matrix2x2<t>::zero = { 0 };

	template<typename t>
	const basic_matrix2x2<t> basic_matrix2x2<t>::identity = basic_matrix2x2<t>(	basic_vec2((t)1, (t)0),
																				basic_vec2((t)0, (t)1) );
	template<typename t>
	const basic_matrix3x3<t> basic_matrix3x3<t>::zero = { 0 };

	template<typename t>
	const basic_matrix3x3<t> basic_matrix3x3<t>::identity = basic_matrix3x3<t>(	basic_vec3((t)1, (t)0, (t)0),
																				basic_vec3((t)0, (t)1, (t)0),
																				basic_vec3((t)0, (t)0, (t)1) );

	template<typename t>
	const basic_matrix4x4<t> basic_matrix4x4<t>::zero = {0};

	template<typename t>
	const basic_matrix4x4<t> basic_matrix4x4<t>::identity = basic_matrix4x4<t>(	basic_vec4((t)1, (t)0, (t)0, (t)0),
																				basic_vec4((t)0, (t)1, (t)0, (t)0),
																				basic_vec4((t)0, (t)0, (t)1, (t)0),
																				basic_vec4((t)0, (t)0, (t)0, (t)1) );

}

#endif
