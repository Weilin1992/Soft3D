#include "Transform.h"


void Transform::set_look_at(const Vec4f &eye, const Vec4f& at, const Vec4f &up) {
	Vec4f zaxis = at - eye;
	zaxis.normalize();
	Vec4f xaxis = up.cross(zaxis);
	xaxis.normalize();
	Vec4f yaxis = zaxis.cross(xaxis);

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

void Transform::set_perspective(float fovy, float aspect, float n, float f) {
	float fax = 1.0f / (float)tan(fovy*0.5f);
	projection.SetZero();
	projection[0][0] = (float)(fax / aspect);
	projection[1][1] = (float)(fax);
	projection[2][2] = f / (f - n);
	projection[3][2] = -n*f / (f - n);
	projection[2][3] = 1;
}

Transform::Transform(int width, int height, float fv) {
	this->fovy = fovy;
	float aspect = (float)width / (float)height;
	set_perspective(3.1415926 * 0.5f, aspect, 1.0f, 500.0f);
	w = (float)width;
	h = (float)height;
	update();
}
Transform::Transform() {}

void Transform::init(int width, int height, float fovy) {
	this->fovy = fovy;
	float aspect = (float)width / (float)height;
	set_perspective(3.1415926 * 0.5f, aspect, 1.0f, 500.0f);
	w = (float)width;
	h = (float)height;
	update();

}



