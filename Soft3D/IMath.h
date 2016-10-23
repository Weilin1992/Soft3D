#pragma once
#include<math.h>

float interp(float x1, float x2, float t);
int CMID(int x, int min, int max);

template<typename T>
class Vector4 {
public:
	Vector4() :x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}
	Vector4(T xx, T yy, T zz, T ww) :x(xx), y(yy), z(zz), w(ww) {}
	
	Vector4(const Vector4<T> &v) :x(v.x), y(v.y), z(v.z), w(v.w) {}
	T x, y, z, w;

	T length() const {
		return (T)sqrt(x*x + y*y + z*z);
	}

	Vector4<T> &normalize() {
		T len = length();
		if (len > 0) {
			T invLen = 1 / len;
			x *= invLen, y *= invLen, z *= invLen;
		}
		return *this;
	}

	T dot(const Vector4<T> &v) const {
		return x*v.x + y*v.y + z*v.z;
	}

	Vector4<T> cross(const Vector4<T> &v) const {
		return Vector4<T>(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x,
			T(1)
			);
	}

	Vector4<T> interp(const Vector4<T> &v, T t) const {
		return Vector4<T>(
			x + (x - v.x) * t,
			y + (y - v.y) * t,
			z + (z - v.z) * t,
			T(1)
			);
	}

	Vector4<T> operator + (const Vector4<T> &v) const {
		return Vector4<T>(x + v.x, y + v.y, z + v.z, T(1));
	}
	Vector4<T> operator - (const Vector4<T> &v) const {
		return Vector4<T>(x - v.x, y - v.y, z - v.z, T(1));
	}
	Vector4<T> operator*(const T &r) const {
		return Vector4<T>(x*r, y*r, z*r, w);
	}
	/*
	Vector4<T> mul(const Matrix44<T> m) const{
		return Vector4<T>(
			x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0],
			x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1],
			x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2],
			x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3]
			);
	}
	*/
};

typedef Vector4<float> Vec4f;

template<typename T>
class Matrix44 {
public:
	Matrix44(){}
	
	T* operator [] (int i) { return m[i]; }
	const T* operator [] (int i) const { return m[i]; }
	Matrix44 operator *(const Matrix44<T>& rhs) const {
		Matrix44 mult;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				mult[i][j] = m[i][0] * rhs[0][j] +
					m[i][1] * rhs[1][j] +
					m[i][2] * rhs[2][j] +
					m[i][3] * rhs[3][j];
			}
		}
		return mult;
	}
	Matrix44 operator -(const Matrix44<T>& rhs) const {
		Matrix44 sub;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; i < 4; j++) {
				mult[i][j] = m[i][j] - rhs[i][j]
			}
		}
		return sub;
	}
	Matrix44 operator +(const Matrix44<T>& rhs) const {
		Matrix44 add;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; i < 4; j++) {
				add[i][j] = m[i][j] + rhs[i][j]
			}
		}
		return add;
	}
	Matrix44 operator *(const T f) const {
		Matrix44 scale;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; i < 4; j++) {
				scale[i][j] = m[i][j] * f;
			}
		}
		return scale;
	}
	void SetIdentity(){
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = T(1);
		m[0][1] = m[0][2] = m[0][3] = T(0);
		m[1][0] = m[1][2] = m[1][3] = T(0);
		m[2][0] = m[2][1] = m[2][3] = T(0);
		m[3][0] = m[3][1] = m[3][2] = T(0);
	}
	void SetZero() {
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = T(0);
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = T(0);
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = T(0);
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = T(0);
	}
	void SetScale(T x, T y, T z) {
		SetIdentity();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
	}




	void SetRotate(T x,T y,T z,T theta) {
		float qsin = (T)sin(theta*0.5f);
		float qcos = (T)cos(theta*0.5f);

		Vector4<T> vec = { x,y,z,T(1) };
		float w = qcos;
		vec.normalize();
		x = vec.x * qsin;
		y = vec.y * qsin;
		z = vec.z * qsin;
		m[0][0] = 1 - 2 * y * y - 2 * z * z;
		m[1][0] = 2 * x * y - 2 * w * z;
		m[2][0] = 2 * x * z + 2 * w * y;
		m[0][1] = 2 * x * y + 2 * w * z;
		m[1][1] = 1 - 2 * x * x - 2 * z * z;
		m[2][1] = 2 * y * z - 2 * w * x;
		m[0][2] = 2 * x * z - 2 * w * y;
		m[1][2] = 2 * y * z + 2 * w * x;
		m[2][2] = 1 - 2 * x * x - 2 * y * y;
		m[0][3] = m[1][3] = m[2][3] =T(0);
		m[3][0] = m[3][1] = m[3][2] = T(0);
		m[3][3] = 1.0f;
		
	}
private:
	T m[4][4] = { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 } };
	int i = 0;

};


typedef Matrix44<float> Mat44f;

Vec4f mul(const Vec4f & v, const Mat44f &m);

typedef unsigned int IUINT32;



inline Vec4f mul(const Vec4f & v, const Mat44f &m) {
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float w = v.w;
	return Vec4f(
		x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0],
		x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1],
		x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2],
		x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3]
		);
}

inline int CMID(int x, int min, int max) {

	return (x < min) ? min : ((x > max) ? max : x);

}





inline float interp(float x1, float x2, float t) {
	return x1 + (x2 - x1)*t;
}