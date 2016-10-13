#pragma once
#include<math.h>


template<typename T>
class Vector4 {
public:
	Vector4() :x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}
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
		return *this；
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

	Vector4<T> operator*(const Matrix44<T> &m) const{
		Vector4<T> res(
			x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0],
			x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1],
			x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2],
			x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3]
			)
		return res;

	}
};

typedef Vector4<float> Vec4f;




template<typename T>
class Matrix44 {
public:
	Matrix44(){}
	
	T* operator[](int i) const{ return m[i]; }
	Matrix44 operator *(const Matrix44<T>& rhs) const {
		Matrix44 mult;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; i < 4; j++) {
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
	
	void Zero() {
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = T(0);
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = T(0);
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = T(0);
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = T(0);
	}

private:
	T m[4][4] = { { 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 } };
	int i = 0;

};


typedef Matrix44<float> Mat44f;


class Transform {
private:
	Mat44f world; //世界坐标系
	Mat44f view;
	Mat44f projection;
	Mat44f transform;
	Vec4f v;
	float w, h;

public:
	Transform() {};
	void init(int width, int height) {
		
	}
	void update() {
		transform = world*view*projection;
	}
	void set_perspective(float fovy, float aspect, float near, float far) {
		float fax = 1.0f / (float)tan(fovy*0.5f);
		projection.Zero();
		projection[0][0] = (float)(fax / aspect);
		projection[1][1] = (float)(fax);
		projection[2][2] = far / (far - near);
		projection[3][2] = -near * far / (far - near);
		projection[2][3] = 1;
	}
};