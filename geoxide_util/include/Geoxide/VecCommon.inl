
static vec_t& add(const vec_t& a, const vec_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] + b[i];

	return result;
}

static vec_t& sub(const vec_t& a, const vec_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] - b[i];

	return result;
}

static vec_t& mul(const vec_t& a, const vec_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] * b[i];

	return result;
}

template <typename scalar_t>
static vec_t& mul(const vec_t& a, const scalar_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] * b;

	return result;
}

static vec_t& div(const vec_t& a, const vec_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] / b[i];

	return result;
}

template <typename scalar_t>
static vec_t& div(const vec_t& a, const scalar_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_components; i++)
		result[i] = a[i] / b;

	return result;
}

template <typename pos_t>
static vec_t& interpolate(const vec_t& a, const vec_t& b, pos_t t, vec_t& result)
{
	result = a + (b - a) * t;

	return result;
}

template <typename length_t>
static vec_t& normalize(const vec_t& a, vec_t& result)
{
	length_t a_len = length(a, a_len);

	result = a / a_len;

	return result;
}

template <typename res_t>
static res_t& dot(const vec_t& a, const vec_t& b, res_t& result)
{
	vec_t ab = a * b;

	for (uint32_t i = 0; i < num_components; i++)
		result += ab[i];

	return result;
}

template <typename res_t>
static res_t& angle(const vec_t& a, const vec_t& b, res_t& result)
{
	res_t d = dot(a, b, d);
	res_t a_length = length(a, a_length);
	res_t b_length = length(b, b_length);

	result = acos(d / (a_length * b_length));

	return result;
}

template <typename res_t>
static res_t& length(const vec_t& a, res_t& result)
{
	result = (res_t)0;

	vec_t aa = a * a;

	for (uint32_t i = 0; i < num_components; i++)
		result += aa[i];

	result = sqrt(result);
	return result;
}

template <typename res_t>
res_t angle(const vec_t& b) const
{
	res_t temp = (t)0;
	return angle(*this, b, temp);
}

template <typename res_t>
res_t dot(const vec_t& b) const
{
	res_t temp = (t)0;
	return dot(*this, b, temp);
}

template <typename res_t>
res_t length() const
{
	res_t temp = (t)0;
	return length(*this, temp);
}

float anglef(const vec_t& b) const { return angle<float>(b); }
float dotf(const vec_t& b) const { return dot<float>(b); }
float lengthf() const { return length<float>(); }

double angled(const vec_t& b) const { return angle<double>(b); }
double dotd(const vec_t& b) const { return dot<double>(b); }
double lengthd() const { return length<double>(); }

template <typename pos_t>
vec_t interpolate(const vec_t& b, pos_t t) const
{
	vec_t temp = zero;
	return interpolate(*this, b, t, temp);
}

vec_t interpolate(const vec_t& b, float t) const { return interpolate<float>(b, t); }
vec_t interpolate(const vec_t& b, double t) const { return interpolate<double>(b, t); }

vec_t& normalize()
{
	return normalize<float>(*this, *this);
}

vec_t get_normalized() const
{
	vec_t temp = zero;
	return normalize<float>(*this, temp);
}

vec_t& normalize_accurate()
{
	return normalize<double>(*this, *this);
}

vec_t get_normalized_accurate() const
{
	vec_t temp = zero;
	return normalize<double>(*this, temp);
}

vec_t operator+(const vec_t& b) const
{
	vec_t temp = zero;
	return add(*this, b, temp);
}

vec_t operator-(const vec_t& b) const
{
	vec_t temp = zero;
	return sub(*this, b, temp);
}

vec_t operator*(const vec_t& b) const
{
	vec_t temp = zero;
	return mul(*this, b, temp);
}

vec_t operator*(float b) const
{
	vec_t temp = zero;
	return mul<float>(*this, b, temp);
}

vec_t operator*(double b) const
{
	vec_t temp = zero;
	return mul<double>(*this, b, temp);
}

vec_t operator/(const vec_t& b) const
{
	vec_t temp = zero;
	return div(*this, b, temp);
}

vec_t operator/(float b) const
{
	vec_t temp = zero;
	return div<float>(*this, b, temp);
}

vec_t operator/(double b) const
{
	vec_t temp = zero;
	return div<double>(*this, b, temp);
}

const t& operator[](size_t i) const { return v[i]; }
t& operator[](size_t i) { return v[i]; }
