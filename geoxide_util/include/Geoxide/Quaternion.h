
#ifndef __GX_QUATERNION_H__
#define __GX_QUATERNION_H__

#include "Vector.h"

namespace Geoxide {

	template <typename t>
	union basic_quat
	{
		static const basic_quat zero;
		static const basic_quat identity;

		basic_quat()
		{
			*this = identity;
		}

		basic_quat(int i)
		{
			assert(i == 0); 
			if (i == 0)
			{
				*this = zero; 
			}
		}

		basic_quat(t x, t y, t z, t w) : x(x), y(y), z(z), w(w) {}

		static basic_quat& axis(const basic_vec3<t>& axis, t rad, basic_quat& result)
		{
			t halfAngle = rad / 2;
			t halfAngleSin = sin(halfAngle);

			result.x = axis.x * halfAngleSin;
			result.y = axis.y * halfAngleSin;
			result.z = axis.z * halfAngleSin;
			result.w = cos(halfAngle);

			return result;
		}

		template <typename pos_t>
		static basic_quat& interpolate(const basic_quat& a, const basic_quat& _b, pos_t t, basic_quat& result)
		{
			pos_t cosom = a.x * _b.x + a.y * _b.y + a.z * _b.z + a.w * _b.w;

			quat b = _b;
			if (cosom < 0.f)
			{
				cosom = -cosom;
				b.x = -b.x;
				b.y = -b.y;
				b.z = -b.z;
				b.w = -b.w;
			}

			pos_t sclp, sclq;

			if ((1.f - cosom) > 0.000001f)
			{
				pos_t omega, sinom;
				omega = acos(cosom);
				sinom = sin(omega);
				sclp = sin((1.f - t) * omega) / sinom;
				sclq = sin(t * omega) / sinom;
			}
			else
			{
				sclp = 1.f - t;
				sclq = t;
			}

			result.x = sclp * a.x + sclq * b.x;
			result.y = sclp * a.y + sclq * b.y;
			result.z = sclp * a.z + sclq * b.z;
			result.w = sclp * a.w + sclq * b.w;

			return result;
		}

		static basic_quat& conjugate(const basic_quat& a, basic_quat& result)
		{
			result.x = -a.x;
			result.y = -a.y;
			result.z = -a.z;
			result.w = a.w;

			return result;
		}

		template <typename pos_t>
		basic_quat interpolate(const basic_quat& b, pos_t t) const
		{
			quat temp = zero;
			return interpolate(*this, b, t, temp);
		}

		basic_quat interpolate(const basic_quat& b, float t) const { return interpolate<float>(b, t); }
		basic_quat interpolate(const basic_quat& b, double t) const { return interpolate<double>(b, t); }

		basic_quat& conjugate()
		{
			return conjugate(*this, *this);
		}

		basic_quat get_conjugated() const
		{
			basic_quat temp = zero;
			return conjugate(*this, temp);
		}

		t q[4];

		struct
		{
			t x, y, z, w;
		};
	};

	template<typename t>
	const basic_quat<t> basic_quat<t>::zero = {};

	template<typename t>
	const basic_quat<t> basic_quat<t>::identity = basic_quat<t>(0, 0, 0, 1);

}

#endif
