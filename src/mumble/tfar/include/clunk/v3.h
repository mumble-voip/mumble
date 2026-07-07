#ifndef CLUNK_V3_H__
#define CLUNK_V3_H__

/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <clunk/types.h>
#include <limits>

namespace clunk {
/*! 
	\brief 3d vector 
	\tparam T type of the axis. usually int or float. 
*/
template <typename T> struct v3  {
	typedef std::numeric_limits<T> limits_type;

	template<typename V>
	static inline V abs(V v) { return v < 0? -v: v; }

	///x component
	T x;
	///y component
	T y;
	///z component
	T z;
	///default ctor: initializes all components with zeroes.
	inline v3<T>() : x(0), y(0), z(0) {}
	///initializes all components with given values
	inline v3<T>(const T x, const T y, const T z) : x(x), y(y), z(z) {} 
	
	///nullify vector
	inline void clear() { x = y = z = 0; }
	///returns true if x == y == z == 0 ? 
	inline bool is0() const {
		T zero = limits_type::epsilon();
		return abs(x) <= zero && abs(y) <= zero && abs(z) <= zero;
	}

	/*! 
		\brief normalizes vector. 
		\return old length of this vector
	*/ 
	inline T normalize() {
		const T len = length();
		if (len == (T)0 || len ==(T)1) 
			return len;
		
		x /= len;
		y /= len;
		z /= len;
		return len;
	}

	/*! 
		\brief normalizes vector with given length
		\param[in] nlen length
		\return old length of this vector
	*/ 

	inline T normalize(const T nlen) {
		const T len = length();
		if (len == (T)0 || len == nlen) 
			return len;
		
		x *= nlen / len;
		y *= nlen / len;
		z *= nlen / len;
		return len;
	}
	
	inline T dot_product(const v3<T> &v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline v3<T> cross_product(const v3<T> &v) const {
		return v3<T>(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}
	
	///returns length of this vector
	inline T length() const {
		const T ql = x * x + y * y + z * z;
		if (ql == (T)0 || ql == (T)1)
				return ql;

		return (T)sqrt(ql);
	}
	///returns square of length. To avoid sqrt if needed.
	inline T quick_length() const {
		return (T)(x * x + y * y + z * z);
	}

	///converts to vector of another type
	template <typename T2> 
		inline v3<T2> convert() const { return v3<T2>((T2)x, (T2)y, (T2)z); }

	///returns distance between two points	
	inline T distance(const v3<T>& other) const {
		v3<T>d(*this);
		d -= other;
		return d.length();
	}
	
	///return square distance between two points
	inline T quick_distance(const v3<T>& other) const {
		const T dx = x - other.x;
		const T dy = y - other.y;
		const T dz = z - other.z;
		return (dx * dx + dy * dy + dz * dz);
	}

	///allows v3 be placed in sorted STL container such std::map
	inline bool operator<(const v3<T> &other) const {
		if (x != other.x) {
			return x < other.x;
		}
		if (y != other.y) {
			return y < other.y;
		} 
		return z < other.z;
	}

	///negate all components
	inline v3<T> operator-() const {
		return v3<T>(-x, -y, -z);
	}
	///test equality 
	inline bool operator==(const v3<T> &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	///test inequality 
	inline bool operator!=(const v3<T> &other) const {
		return x != other.x || y != other.y || z != other.z;
	}
	
	///adds another vector
	inline v3<T>& operator+=(const v3<T>& other) {
		x += other.x; y += other.y; z += other.z;
		return *this;
	}

	///substracts another vector
	inline v3<T>& operator-=(const v3<T>& other) {
		x -= other.x; y -= other.y; z -= other.z;
		return *this;
	}

	///multiplies another vector
	inline v3<T>& operator*=(const v3<T>& other) {
		x *= other.x; y *= other.y; z *= other.z;
		return *this;
	}

	///divide with another vector
	inline v3<T>& operator/=(const v3<T>& other) {
		x /= other.x; y /= other.y; z /= other.z;
		return *this;
	}
	///multiplication
	inline v3<T> operator*(const v3<T>& other) const {
		return v3<T>(x * other.x, y * other.y, z * other.z);
	}
	///summing
	inline v3<T> operator+(const v3<T>& other) const {
		return v3<T>(x + other.x, y + other.y, z + other.z);
	}
	///substraction
	inline v3<T> operator-(const v3<T>& other) const {
		return v3<T>(x - other.x, y - other.y, z - other.z);
	}
	///division
	inline v3<T> operator/(const v3<T>& other) const {
		return v3<T>(x / other.x, y / other.y, z / other.z);
	}
	///multiplies all components with constant
	inline v3<T> operator*(const T& other) const {
		return v3<T>(x * other, y * other, z * other);
	}
	///sums all components with constant
	inline v3<T> operator+(const T& other) const {
		return v3<T>(x + other, y + other, z + other);
	}
	///substracts all components with constant
	inline v3<T> operator-(const T& other) const {
		return v3<T>(x - other, y - other, z - other);
	}
	///divides all components by constant
	inline v3<T> operator/(const T& other) const {
		return v3<T>(x / other, y / other, z / other);
	}
	///divides this vector by constant
	inline v3<T>& operator/=(const T& other) {
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	///multiplies this vector with constant
	inline v3<T>& operator*=(const T& other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	///sums this vector with constant
	inline v3<T>& operator+=(const T& other) {
		x += other;
		y += other;
		z += other;
		return *this;
	}

	///substracts this vector with constant
	inline v3<T>& operator-=(const T& other) {
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	///adds constant to the vector
	inline v3<T> operator+(const T a)  {
		return v3<T>(x + a, y + a, z + a);
	}

	///subs constant from the vector
	inline v3<T> operator-(const T a)  {
		return v3<T>(x - a, y - a, z - a);
	}

	///muls constant to the vector
	inline v3<T> operator*(const T a)  {
		return v3<T>(x * a, y * a, z * a);
	}

	///divs constant to the vector
	inline v3<T> operator/(const T a)  {
		return v3<T>(x / a, y  / a, z / a);
	}
};
///external operator+
template <typename T>
	inline v3<T> operator+(const T a, const v3<T> &v)  {
		return v3<T>(v.x + a, v.y + a, v.z + a);
	}

///external operator-
template <typename T>
	inline v3<T> operator-(const T a, const v3<T> &v)  {
		return v3<T>(a - v.x, a - v.y, a - v.z);
	}

///external operator*
template <typename T>
	inline v3<T> operator*(const T a, const v3<T> &v)  {
		return v3<T>(v.x * a, v.y * a, v.z * a);
	}

///external operator/
template <typename T>
	inline v3<T> operator/(const T a, const v3<T> &v)  {
		return v3<T>(a / v.x, a / v.y, a / v.z);
	}

typedef v3<float> v3f;

#if 0 //defined CLUNK_USES_SSE
#	include <xmmintrin.h>

template <> 
class v3<float>  {
	__m128 value;
	
public:

	template <int POS>
	class float_placeholder {
		__m128 &value;
	public:
		inline float_placeholder(__m128 &value) __attribute__((__always_inline__))  : value(value) {}
		inline float get() const __attribute__((__always_inline__)) {
			float r[4]; 
			_mm_storeu_ps(r, value);
			return r[POS];
		}
		inline void set(float v) __attribute__((__always_inline__)) {
			__m128 mask = _mm_cmpgt_ps(_mm_set_ps(POS == 2?1:0, POS == 1?1:0, POS == 0?1:0, 0), _mm_setzero_ps());
			value = _mm_or_ps(
				_mm_andnot_ps(mask, value), 
				_mm_and_ps(mask, _mm_set1_ps(v))
			);
		}
		
		inline operator float() const __attribute__((__always_inline__)) {
			return get();
		}
		inline const float_placeholder<POS>& operator=(const float_placeholder<POS> &other) __attribute__((__always_inline__)) {
			set(other.get());
			return *this;
		}

		inline const float_placeholder<POS>& operator=(float v) __attribute__((__always_inline__)) {
			set(v);
			return *this;
		}
	};
	float_placeholder<0> x;
	float_placeholder<1> y;
	float_placeholder<2> z;

	inline v3<float>(): value(_mm_setzero_ps()), x(value), y(value), z(value) {}
	inline v3<float>(__m128 v): value(v), x(value), y(value), z(value) {}
	inline v3<float>(const float xv, const float yv, const float zv) : 
		value(_mm_set_ps(zv, yv, xv, 0)), x(value), y(value), z(value) {}
	
	inline void clear() { value = _mm_setzero_ps(); }

	inline const bool is0() const {
		return (_mm_movemask_ps(_mm_cmpeq_ps(value, _mm_setzero_ps())) & 7 ) == 7;
	}

	inline const float normalize() {
		float len = length();
		if (len == (float)0 || len ==(float)1)
			return len;
			
		value = _mm_div_ps(value, _mm_set1_ps(len));
		return len;
	}

	inline const float normalize(const float nlen) {
		const float len = length();
		if (len == (float)0 || len == nlen) 
			return len;
		
		__m128 k = _mm_div_ps(_mm_set1_ps(nlen), _mm_set1_ps(len));
		value = _mm_div_ps(value, k);
		return len;
	}
	
	inline const float dot_product(const v3<float> &v) const {
		float fixme[4];
		_mm_storeu_ps(fixme, _mm_mul_ps(value, v.value));
		return fixme[0] + fixme[1] + fixme[2] + fixme[3];
	}
	
	inline const float length() const {
		float v[4];
		_mm_storeu_ps(v, _mm_mul_ps(value, value));
		const float ql = v[0] + v[1] + v[2];
		if (ql == 0 || ql == 1.0f) 
			return ql;
		
		return (float)sqrt(ql);
	}

	inline float quick_length() const {
		float v[4];
		_mm_storeu_ps(v, _mm_mul_ps(value, value));
		return v[0] + v[1] + v[2];
	}

	template <typename T2> 
		inline v3<T2> convert() const { 
			float v[4];
			_mm_storeu_ps(v, value);
			return v3<T2>((T2)v[0], (T2)v[1], (T2)v[2]); 
		}

	inline const float distance(const v3<float>& other) const {
		v3<float>d(*this);
		d -= other;
		return d.length();
	}
	
	inline float quick_distance(const v3<float>& other) const {
		float v[4];
		__m128 dv = _mm_sub_ps(value, other.value);
		_mm_storeu_ps(v, _mm_mul_ps(dv, dv));
		return v[0] + v[1] + v[2];
	}

	inline const bool operator<(const v3<float> &other) const {
		int c = _mm_movemask_ps(_mm_cmpneq_ps(value, other.value));
		int l = _mm_movemask_ps(_mm_cmplt_ps(value, other.value));
		if (c & 1) {
			return l & 1;
		}
		if (c & 2) {
			return l & 2;
		} 
		return l & 4;
	}

	inline const v3<float> operator-() const {
		return _mm_sub_ps(_mm_setzero_ps(), value);
	}

	inline const bool operator==(const v3<float> &other) const {
		int mask = _mm_movemask_ps(_mm_cmpeq_ps(value, other.value));
		return (mask & 7) == 7;
	}

	///test inequality 
	inline const bool operator!=(const v3<float> &other) const {
		int mask = _mm_movemask_ps(_mm_cmpeq_ps(value, other.value));
		return (mask & 7) != 7;
	}
	
	///adds another vector
	inline const v3<float>& operator+=(const v3<float>& other) {
		value = _mm_add_ps(value, other.value);
		return *this;
	}

	///substracts another vector
	inline const v3<float>& operator-=(const v3<float>& other) {
		value = _mm_sub_ps(value, other.value);
		return *this;
	}

	///multiplies another vector
	inline const v3<float>& operator*=(const v3<float>& other) {
		value = _mm_mul_ps(value, other.value);
		return *this;
	}

	///divide with another vector
	inline const v3<float>& operator/=(const v3<float>& other) {
		value = _mm_div_ps(value, other.value);
		return *this;
	}
	///multiplication
	inline const v3<float> operator*(const v3<float>& other) const {
		return v3<float>(_mm_mul_ps(value, other.value));
	}
	///summing
	inline const v3<float> operator+(const v3<float>& other) const {
		return v3<float>(_mm_add_ps(value, other.value));
	}
	///substraction
	inline const v3<float> operator-(const v3<float>& other) const {
		return v3<float>(_mm_sub_ps(value, other.value));
	}
	///division
	inline const v3<float> operator/(const v3<float>& other) const {
		return v3<float>(_mm_div_ps(value, other.value));
	}
	///multiplies all components with constant
	inline const v3<float> operator*(const float& other) const {
		return v3<float>(_mm_mul_ps(value, _mm_set1_ps(other)));
	}
	///sums all components with constant
	inline const v3<float> operator+(const float& other) const {
		return v3<float>(_mm_add_ps(value, _mm_set1_ps(other)));
	}
	///substracts all components with constant
	inline const v3<float> operator-(const float& other) const {
		return v3<float>(_mm_sub_ps(value, _mm_set1_ps(other)));
	}
	///divides all components by constant
	inline const v3<float> operator/(const float& other) const {
		return v3<float>(_mm_div_ps(value, _mm_set1_ps(other)));
	}
	///divides this vector by constant
	inline const v3<float>& operator/=(const float& other) {
		value = _mm_div_ps(value, _mm_set1_ps(other));
		return *this;
	}

	///multiplies this vector with constant
	inline const v3<float>& operator*=(const float& other) {
		value = _mm_mul_ps(value, _mm_set1_ps(other));
		return *this;
	}

	///sums this vector with constant
	inline const v3<float>& operator+=(const float& other) {
		value = _mm_add_ps(value, _mm_set1_ps(other));
		return *this;
	}

	///substracts this vector with constant
	inline const v3<float>& operator-=(const float& other) {
		value = _mm_sub_ps(value, _mm_set1_ps(other));
		return *this;
	}

	///adds constant to the vector
	inline const v3<float> operator+(const float a)  {
		return v3<float>(_mm_add_ps(value, _mm_set1_ps(a)));
	}

	///subs constant from the vector
	inline const v3<float> operator-(const float a)  {
		return v3<float>(_mm_sub_ps(value, _mm_set1_ps(a)));
	}

	///muls constant to the vector
	inline const v3<float> operator*(const float a)  {
		return v3<float>(_mm_mul_ps(value, _mm_set1_ps(a)));
	}

	///divs constant to the vector
	inline const v3<float> operator/(const float a)  {
		return v3<float>(_mm_div_ps(value, _mm_set1_ps(a)));
	}
};

#endif //CLUNK_USES_SSE
	
} //namespace clunk

#endif

