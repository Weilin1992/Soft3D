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
	float fovy;
public:
	Transform() {

	}
public:

	void init(int width, int height,float fovy) {
		this->fovy = fovy;
		float aspect = (float)width / (float)height;
		set_perspective(3.1415926 * 0.5f,aspect,1.0f,500.0f);
		w = (float)width;
		h = (float)height;
		update();
		
	}

	void update() {
		transform = world*view*projection;
	}
	void set_perspective(float fovy, float aspect, float near, float far) {
		float fax = 1.0f / (float)tan(fovy*0.5f);
		projection.SetZero();
		projection[0][0] = (float)(fax / aspect);
		projection[1][1] = (float)(fax);
		projection[2][2] = far / (far - near);
		projection[3][2] = -near * far / (far - near);
		projection[2][3] = 1;
	}
	void set_look_at(const Vec4f &eye, const Vec4f& at, const Vec4f &up) {
		Vec4f zaxis = at - eye;
		zaxis.normalize();
		Vec4f xaxis = up.cross(zaxis);
		xaxis.normalize();
		Vec4f yaxis = xaxis.cross(zaxis);

		view[0][0] = xaxis.x;
		view[1][0] = xaxis.y;
		view[2][0] = xaxis.z;
		view[3][0] = -xaxis.dot(eye);

		view[0][1] = yaxis.x;
		view[1][1] = yaxis.y;
		view[2][1] = yaxis.z;
		view[3][1] = -yaxis.dot(eye);

		view[0][2] = zaxis.x;
		view[1][2] = zaxis.y;
		view[2][2] = zaxis.z;
		view[3][2] = -zaxis.dot(eye);

		view[0][3] = view[1][3] = view[2][3] = 0.0f;
		view[3][3] = 1.0f;






	}
	
	Vec4f apply(Vec4f v) {
		return v*transform;
	}

	int check_cvv(const Vec4f& v) const{
		float w = v.w;
		int check = 0;
		if (v.z < 0.0f) check |= 1;
		if (v.z >  w) check |= 2;
		if (v.x < -w) check |= 4;
		if (v.x >  w) check |= 8;
		if (v.y < -w) check |= 16;
		if (v.y >  w) check |= 32;
		return check;
	}

	Vec4f homogenize(const Vec4f& x) {
		float rhw = 1.0f / x.w;
		return Vec4f(
			(x.x * rhw + 1.0f) * w * 0.5f,
			(1.0f - x.y * rhw) * h * 0.5f,
			x.z*rhw,
			1.0f
		);
	}


};

typedef unsigned int IUINT32;

int CMID(int x, int min, int max) {

	return (x < min) ? min : ((x > max) ? max : x);

}