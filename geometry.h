#pragma once
//
//#include <cmath>
//#include <cassert>
//#include <vector>
//#include <iostream>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//template <class t> struct Vec2 {
//	union {
//		struct { t u, v; };
//		struct { t x, y; };
//		t raw[2];
//	};
//	Vec2() : u(0), v(0) {}
//	Vec2(t _u, t _v) : u(_u), v(_v) {}
//	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
//	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }
//	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u * f, v * f); }
//	inline t& operator[](int i)					{ return raw[i]; }
//	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
//};
//
//template <class t> struct Vec4;
//
//template <class t> struct Vec3 {
//	union {
//		struct { t x, y, z; };
//		struct { t ivert, iuv, inorm; };
//		t raw[3];
//	};
//	Vec3() : x(0), y(0), z(0) {}
//	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
//	Vec3(const Vec4<t>& vec4) : x(vec4.x), y(vec4.y), z(vec4.z) {}
//	static Vec3<t> cross(const Vec3<t>& v1, const Vec3<t>& v2) { return Vec3<t>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); }
//	inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
//	inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
//	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
//	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
//	inline t&	   operator[](int i)			      { return raw[i]; }
//	inline const t& operator[](int i) const { return raw[i]; }
//	float length() const { return std::sqrt(x * x + y * y + z * z); }
//	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / length()); return *this; }
//	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
//};
//
//template <class t> struct Vec4 {
//	union {
//		struct { t x, y, z, w; };
//		t raw[4];
//	};
//	Vec4() : x(0), y(0), z(0), w(1.0) {}
//	Vec4(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
//	Vec4(const Vec3<t>& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0) {}
//	inline Vec4<t> operator +(const Vec4<t>& v) const { return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w); }
//	inline Vec4<t> operator -(const Vec4<t>& v) const { return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w); }
//	inline Vec4<t> operator *(float f)          const { return Vec4<t>(x * f, y * f, z * f, w * f); }
//	inline t       operator *(const Vec4<t>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
//	inline t& operator[](int i) { return raw[i]; }
//	inline const t& operator[](int i) const { return raw[i]; }
//	float length() const { return std::sqrt(x * x + y * y + z * z + w*w); }
//	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / length()); return *this; }
//	template <class > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
//};
//
//
//template <int n, class t> struct vec{
//	union {
//		struct { t x, y, z, w; };
//		t raw[n];
//	};
//	vec() : x(0), y(0), z(0), w(1.0) {}
//	vec(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
//	vec(const Vec3<t>& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0) {}
//	inline Vec4<t> operator +(const Vec4<t>& v) const { return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w); }
//	inline Vec4<t> operator -(const Vec4<t>& v) const { return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w); }
//	inline Vec4<t> operator *(float f)          const { return Vec4<t>(x * f, y * f, z * f, w * f); }
//	inline t       operator *(const Vec4<t>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
//	inline t& operator[](int i) { return raw[i]; }
//	inline const t& operator[](int i) const { return raw[i]; }
//	float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
//	vec<n, t>& normalize(t l = 1) { *this = (*this) * (l / length()); return *this; }
//	template <int n, class t> friend std::ostream& operator<<(std::ostream& s, vec<n, t>& v);
//};
//
//typedef vec<2, float> vec2;
//typedef vec<2, int>   vec2;
//typedef vec<3, float> vec3;
//typedef vec<3, int>   Vec3i;
//typedef vec<4, float> vec4;
//typedef vec<4, int>   Vec4i;
//
//template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
//	s << "(" << v.x << ", " << v.y << ")\n";
//	return s;
//}
//
//template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
//	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
//	return s;
//}
//
//template <int n, class t> std::ostream& operator<<(std::ostream& s, vec<n, t>& v) {
//	return s;
//}
//
//template <int row, int col> 
//class mat {
//public:
//	mat(int r = row, int c = col);
//
//	static mat identity();
//	std::vector<float>& operator[](const int i);
//	vec3  operator*(const vec3& a);
//	vec4  operator*(const vec4& a);
//	mat transpose();
//	mat inverse();
//
//    template <int row, int col> friend std::ostream& operator<<(std::ostream& s, mat& m);
//
//public:
//	std::vector<std::vector<float> > m;
//};
//
//template <int row, int col>
//mat<row, col>::mat(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))) { }
//
//template <int row, int col>
//mat<row, col> mat<row, col>::identity() {
//    mat<row, col> E;
//    for (int i = 0; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            E[i][j] = (i == j ? 1.f : 0.f);
//        }
//    }
//    return E;
//}
//
//template <int row, int col>
//std::vector<float>& mat<row, col>::operator[](const int i) {
//    assert(i >= 0 && i < row);
//    return m[i];
//}
//
//template <int row, int col>
//vec3 mat<row, col>::operator*(const vec3& a) {
//    assert(col == 3);
//    vec3 result;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            result[i] += m[i][j] * a[j];
//        }
//    }
//    return result;
//}
//
//template <int row, int col>
//vec4 mat<row, col>::operator*(const vec4& a) {
//    assert(col == 4);
//    vec4 result;
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 4; j++) {
//            result[i] += m[i][j] * a[j];
//        }
//    }
//    return result;
//}
//
//template <int row, int col>
//mat<row, col> mat<row, col>::transpose() {
//    mat result(col, row);
//    for (int i = 0; i < row; i++)
//        for (int j = 0; j < col; j++)
//            result[j][i] = m[i][j];
//    return result;
//}
//
//template <int row, int col>
//mat<row, col> mat<row, col>::inverse() {
//    assert(row == col);
//    // augmenting the square mat with the identity mat of the same dimensions a => [ai]
//    mat result(row, col * 2);
//    for (int i = 0; i < row; i++)
//        for (int j = 0; j < col; j++)
//            result[i][j] = m[i][j];
//    for (int i = 0; i < row; i++)
//        result[i][i + col] = 1;
//    // first pass
//    for (int i = 0; i < row - 1; i++) {
//        // normalize the first row
//        for (int j = result.col - 1; j >= 0; j--)
//            result[i][j] /= result[i][i];
//        for (int k = i + 1; k < row; k++) {
//            float coeff = result[k][i];
//            for (int j = 0; j < result.col; j++) {
//                result[k][j] -= result[i][j] * coeff;
//            }
//        }
//    }
//    // normalize the last row
//    for (int j = result.col - 1; j >= row - 1; j--)
//        result[row - 1][j] /= result[row - 1][row - 1];
//    // second pass
//    for (int i = row - 1; i > 0; i--) {
//        for (int k = i - 1; k >= 0; k--) {
//            float coeff = result[k][i];
//            for (int j = 0; j < result.col; j++) {
//                result[k][j] -= result[i][j] * coeff;
//            }
//        }
//    }
//    // cut the identity mat back
//    mat truncate(row, col);
//    for (int i = 0; i < row; i++)
//        for (int j = 0; j < col; j++)
//            truncate[i][j] = result[i][j + col];
//    return truncate;
//}
//
//template <int row, int col>
//std::ostream& operator<<(std::ostream& s, mat<row, col>& m) {
//    for (int i = 0; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            s << m[i][j];
//            if (j < col - 1) s << "\t";
//        }
//        s << "\n";
//    }
//    return s;
//}
//
//template<int R1, int C1, int C2>mat<R1, C2> operator*(const mat<R1, C1>& lhs, const mat<C1, C2>& rhs) {
//	mat<R1, C2> result;
//	for (int i = 0; i < R1; i++) {
//		for (int j = 0; j < C2; j++) {
//			result.m[i][j] = 0.f;
//			for (int k = 0; k < C1; k++) {
//				result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
//			}
//		}
//	}
//	return result;
//}

#include <cmath>
#include <cassert>
#include <iostream>

template<int n> struct vec {
	vec() = default;
	double& operator[](const int i) { assert(i >= 0 && i < n); return data[i]; }
	double   operator[](const int i) const { assert(i >= 0 && i < n); return data[i]; }
	double length() const { return std::sqrt((*this) * (*this)); }
	double data[n] = { 0 };
};

template<int n> double operator*(const vec<n>& lhs, const vec<n>& rhs) {
	double ret = 0;
	for (int i = n; i--; ret += lhs[i] * rhs[i]);
	return ret;
}

template<int n> vec<n> operator+(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] += rhs[i]);
	return ret;
}

template<int n> vec<n> operator-(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] -= rhs[i]);
	return ret;
}

template<int n> vec<n> operator-(const vec<n>& lhs, const double& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] -= rhs);
	return ret;

}

template<int n> vec<n> operator*(const double& rhs, const vec<n>& lhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] *= rhs);
	return ret;
}

template<int n> vec<n> operator*(const vec<n>& lhs, const double& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] *= rhs);
	return ret;
}

template<int n> vec<n> operator/(const vec<n>& lhs, const double& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] /= rhs);
	return ret;
}

template<int n1, int n2> vec<n1> embed(const vec<n2>& v, double fill = 1) {
	vec<n1> ret;
	for (int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
	return ret;
}

template<int n1, int n2> vec<n1> proj(const vec<n2>& v) {
	vec<n1> ret;
	for (int i = n1; i--; ret[i] = v[i]);
	return ret;
}


template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v) {
	for (int i = 0; i < n; i++) out << v[i] << " ";
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<> struct vec<2> {
	vec() = default;
	vec(double X, double Y) : x(X), y(Y) {}
	double& operator[](const int i) { assert(i >= 0 && i < 2); return i == 0 ? x : y; }
	double  operator[](const int i) const { assert(i >= 0 && i < 2); return i == 0 ? x : y; }
	double length() const { return std::sqrt((*this) * (*this)); }
	vec& normalize() { *this = (*this) / length(); return *this; }

	union 
	{
		struct { double x, y; };
		struct { double u, v; };
	};
	//double x{}, y{};
};

/////////////////////////////////////////////////////////////////////////////////
//template<> struct vec<4>;

template<> struct vec<3> {
	vec() = default;
	vec(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
	//vec(vec<4> vec4) : x(vec4.x), y(vec4.y), z(vec4.z) {}
	static vec<3> cross(const vec<3>& v1, const vec<3>& v2) {
		return vec<3>{v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x};
	}
	double& operator[](const int i) { assert(i >= 0 && i < 3); return i == 0 ? x : (1 == i ? y : z); }
	double  operator[](const int i) const { assert(i >= 0 && i < 3); return i == 0 ? x : (1 == i ? y : z); }
	double length() const { return std::sqrt((*this) * (*this)); }
	vec& normalize() { *this = (*this) / length(); return *this; }

	double x{}, y{}, z{};
};

template<> struct vec<4> {
	vec() = default;
	vec(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
	vec(vec<3> vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f) {}

	double& operator[](const int i) { assert(i >= 0 && i < 4); return data[i]; }
	double  operator[](const int i) const { assert(i >= 0 && i < 4); return data[i]; }
	double length() const { return std::sqrt((*this) * (*this)); }
	vec& normalize() { *this = (*this) / length(); return *this; }

	union
	{
		struct { double x, y, z, w; };
		double data[4];
	};
};
/////////////////////////////////////////////////////////////////////////////////

template<int n> struct dt;

template<int nrows, int ncols> struct mat {
	vec<ncols> rows[nrows] = { {} };

	mat() = default;
	vec<ncols>& operator[] (const int idx) { assert(idx >= 0 && idx < nrows); return rows[idx]; }
	const vec<ncols>& operator[] (const int idx) const { assert(idx >= 0 && idx < nrows); return rows[idx]; }

	vec<nrows> col(const int idx) const {
		assert(idx >= 0 && idx < ncols);
		vec<nrows> ret;
		for (int i = nrows; i--; ret[i] = rows[i][idx]);
		return ret;
	}

	void set_col(const int idx, const vec<nrows>& v) {
		assert(idx >= 0 && idx < ncols);
		for (int i = nrows; i--; rows[i][idx] = v[i]);
	}

	static mat<nrows, ncols> identity() {
		mat<nrows, ncols> ret;
		for (int i = nrows; i--; )
			for (int j = ncols; j--; ret[i][j] = (i == j));
		return ret;
	}

	double det() const {
		return dt<ncols>::det(*this);
	}

	mat<nrows - 1, ncols - 1> get_minor(const int row, const int col) const {
		mat<nrows - 1, ncols - 1> ret;
		for (int i = nrows - 1; i--; )
			for (int j = ncols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);
		return ret;
	}

	double cofactor(const int row, const int col) const {
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	mat<nrows, ncols> adjugate() const {
		mat<nrows, ncols> ret;
		for (int i = nrows; i--; )
			for (int j = ncols; j--; ret[i][j] = cofactor(i, j));
		return ret;
	}

	mat<nrows, ncols> invert_transpose() const {
		mat<nrows, ncols> ret = adjugate();
		return ret / (ret[0] * rows[0]);
	}

	mat<nrows, ncols> invert() const {
		return invert_transpose().transpose();
	}

	mat<ncols, nrows> transpose() const {
		mat<ncols, nrows> ret;
		for (int i = ncols; i--; ret[i] = this->col(i));
		return ret;
	}
};

/////////////////////////////////////////////////////////////////////////////////

template<int nrows, int ncols> vec<nrows> operator*(const mat<nrows, ncols>& lhs, const vec<ncols>& rhs) {
	vec<nrows> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] * rhs);
	return ret;
}

template<int R1, int C1, int C2>mat<R1, C2> operator*(const mat<R1, C1>& lhs, const mat<C1, C2>& rhs) {
	mat<R1, C2> result;
	for (int i = R1; i--; )
		for (int j = C2; j--; result[i][j] = lhs[i] * rhs.col(j));
	return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator*(const mat<nrows, ncols>& lhs, const double& val) {
	mat<nrows, ncols> result;
	for (int i = nrows; i--; result[i] = lhs[i] * val);
	return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator/(const mat<nrows, ncols>& lhs, const double& val) {
	mat<nrows, ncols> result;
	for (int i = nrows; i--; result[i] = lhs[i] / val);
	return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator+(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> result;
	for (int i = nrows; i--; )
		for (int j = ncols; j--; result[i][j] = lhs[i][j] + rhs[i][j]);
	return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator-(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> result;
	for (int i = nrows; i--; )
		for (int j = ncols; j--; result[i][j] = lhs[i][j] - rhs[i][j]);
	return result;
}

template<int nrows, int ncols> std::ostream& operator<<(std::ostream& out, const mat<nrows, ncols>& m) {
	for (int i = 0; i < nrows; i++) out << m[i] << std::endl;
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<int n> struct dt {
	static double det(const mat<n, n>& src) {
		double ret = 0;
		for (int i = n; i--; ret += src[0][i] * src.cofactor(0, i));
		return ret;
	}
};

template<> struct dt<1> {
	static double det(const mat<1, 1>& src) {
		return src[0][0];
	}
};

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;
//typedef vec<2> vec2;
//typedef vec<2> vec2;
//typedef vec<3> vec3;
//typedef vec<3> Vec3i;
//typedef vec<4> vec4;
//typedef vec<4> Vec4i;


