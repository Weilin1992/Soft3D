#pragma once
#include<math.h>


template<typename T>
class Vector4 {
public:
	Vector4() :x(T(0)), y(T(0), z(T(0), w(T(1)) {}
	Vector4(T xx, T yy, T zz, T ww) :x(xx), y(yy), z(zz), w(ww) {}
	T x, y, z, w;
	Vector4(const Vector4 &v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

	T length() const {
		return (T)sqrt(x*x + y*y + z*z);
	}

	Vector4<T> &normalize() {
		T len = length();
		if (len > 0) {
			T invLen = 1 / len;
			x *= invLen, y *= invLen, z *= invLen;
		}
		return *this
	}

	T dot(const Vector4<T> &v) const {
		return x*v.x + y*v.y + z*v.z;
	}

	Vector4<T> cross(const Vector4<T> &v) const {
		return Vector4<T>(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x,
			T(1),
			);
	}

	Vector4<T> interp(const &Vector4<T> v, T t) const {
		return Vector4<T>(
			x + (x - v.x) * t,
			y + (y - v.y) * t,
			z + (z - v.z) * t,
			T(1),
			);
	}

	Vector4<T> operator + (const Vector4<T> &v) const {
		return Vector4<T>(x + v.x, y + v.y, z + v.z, T(1));
	}
	Vector4<T> operator - (const Vector4<T> &v) const {
		return Vector4<T>(x - v.x, y - v.y, z - v.z, T(1));
	}
	Vector4<T> operator*(const T &r) const {
		return Vector4(x*r, y*r, z*r, w);
	}

};


