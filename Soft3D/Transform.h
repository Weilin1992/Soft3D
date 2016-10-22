#pragma once
#include "IMath.h"

class Transform {
public:
	Mat44f world; //世界坐标系
	Mat44f view;
	Mat44f projection;
	Mat44f transform;
	Vec4f v;
	float w, h;
	float fovy;
public:
	Transform();

	Transform(int width, int height, float fv);
public:

	void init(int width, int height, float fovy);
	void set_perspective(float fovy, float aspect, float n, float f);
	void set_look_at(const Vec4f &eye, const Vec4f& at, const Vec4f &up);

	void update();

	Vec4f apply(Vec4f v);

	int check_cvv(const Vec4f& v) const;

	Vec4f homogenize(const Vec4f& x) const;
};


inline void Transform::update() {
	transform = world*view*projection;
}

inline Vec4f Transform::apply(Vec4f v) {
	return mul(v, transform);
}

inline int Transform::check_cvv(const Vec4f& v) const {
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

inline Vec4f Transform::homogenize(const Vec4f& x) const {
	float rhw = 1.0f / x.w;
	return Vec4f(
		(x.x * rhw + 1.0f) * w * 0.5f,
		(1.0f - x.y * rhw) * h * 0.5f,
		x.z*rhw,
		1.0f
		);
}
