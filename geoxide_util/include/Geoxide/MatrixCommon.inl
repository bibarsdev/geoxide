
static matrix_t& mul(const matrix_t& a, const matrix_t& b, matrix_t& result)
{
	for (uint32_t i = 0; i < num_rows; ++i)
		for (uint32_t f = 0; f < b.num_columns; ++f)
			for (uint32_t j = 0; j < num_columns; ++j)
				result[i][f] += a[i][j] * b[j][f];

	return result;
}

static vec_t& mul(const matrix_t& a, const vec_t& b, vec_t& result)
{
	for (uint32_t i = 0; i < num_rows; ++i)
			for (uint32_t j = 0; j < num_columns; ++j)
				result[i] += a[i][j] * b[j];

	return result;
}

static bool is_equal(const matrix_t& a, const matrix_t& b)
{
	for (uint32_t i = 0; i < num_rows; ++i)
		for (uint32_t j = 0; j < num_columns; ++j)
			if (a[i][j] != b[i][j])
				return false;

	return true;
}

static matrix_t& transpose(const matrix_t& a, matrix_t& result)
{
	for (uint32_t i = 0; i < num_columns; ++i)
			for (uint32_t j = 0; j < num_rows; ++j)
				result[j][i] = a[i][j];

	return result;
}

matrix_t& transpose()
{
	matrix_t temp = zero;
	*this = transpose(*this, temp);
	return *this;
}

matrix_t get_transposed() const
{
	matrix_t temp = zero;
	return transpose(*this, temp);
}

matrix_t operator*(const matrix_t& b) const
{
	matrix_t temp = zero;
	return mul(*this, b, temp);
}

vec_t operator*(const vec_t& b) const
{
	vec_t temp = vec_t::zero;
	return mul(*this, b, temp);
}

bool operator==(const matrix_t& b) const
{
	return is_equal(*this, b);
}

const vec_t& operator[](size_t i) const { return r[i]; }
vec_t& operator[](size_t i) { return r[i]; }
