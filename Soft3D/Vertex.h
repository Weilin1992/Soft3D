#pragma once
#include "IMath.h"

typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
typedef Vec4f point_t;
struct Face {
	int i1, i2, i3;
};
class Vertex {
public:
	point_t pos;
	texcoord_t tc;
	color_t color;
	float rhw;
	Vec4f normal;

public:
	Vertex() {}
	Vertex(point_t pos,color_t color, texcoord_t tc):
	pos(pos),color(color),tc(tc)
	{
		rhw = 1.0f/ pos.w;
		rhw_init();
	}

	Vertex(const Vertex& v) {
		pos = v.pos;
		color = v.color;
		tc = v.tc;
		rhw = v.rhw;
		normal = v.normal;
	}

	void rhw_init() {
		rhw = 1.0f / pos.w;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	}

	
	static Vertex division(const Vertex& v1, const Vertex& v2, float w) {
		float inv = 1.0f / w;
		Vertex res;
		res.pos.x = (v2.pos.x - v1.pos.x)*inv;
		res.pos.y = (v2.pos.y - v1.pos.y)*inv;
		res.pos.z = (v2.pos.z - v1.pos.z)*inv;
		res.pos.w = (v2.pos.w - v1.pos.w)*inv;

		res.normal = (v1.normal - v2.normal)*inv;

		res.tc.u = (v2.tc.u - v1.tc.u)*inv;
		res.tc.v = (v2.tc.v - v1.tc.v)*inv;

		res.color.r = (v2.color.r - v1.color.r)*inv;
		res.color.g = (v2.color.g - v1.color.g)*inv;
		res.color.b = (v2.color.b - v1.color.b)*inv;

		res.rhw = (v2.rhw - v1.rhw) * inv;
		return res;
	}

	void add(const Vertex &v) {
		this->pos = v.pos + this->pos;
		this->normal = v.normal + this->normal;
		this->tc.u = v.tc.u + this->tc.u;
		this->tc.v = v.tc.v + this->tc.v;
		this->color.r = v.color.r + this->color.r;
		this->color.b = v.color.b + this->color.b;
		this->color.g = v.color.g + this->color.g;
		this->rhw = v.rhw + this->rhw;
	}
	
	static Vertex Interp(const Vertex &v1,const Vertex &v2,float t) {
		Vertex res;
		res.pos.x = interp(v1.pos.x, v2.pos.x, t);
		res.pos.y= interp(v1.pos.y, v2.pos.y, t);
		res.pos.z = interp(v1.pos.z, v2.pos.z, t);

		res.normal.x = interp(v1.normal.x, v2.normal.x, t);
		res.normal.y = interp(v1.normal.y, v2.normal.y, t);
		res.normal.z = interp(v1.normal.z, v2.normal.z, t);
		res.normal.w = 0.0f;
		res.normal.normalize();

		res.pos.w = interp(v1.pos.w, v2.pos.w, t);
		res.tc.u = interp(v1.tc.u, v2.tc.u, t);
		res.tc.v = interp(v1.tc.v, v2.tc.v, t);
		res.color.r = interp(v1.color.r, v2.color.r, t);
		res.color.g = interp(v1.color.g, v2.color.g, t);
		res.color.b = interp(v1.color.b, v2.color.b, t);
		res.rhw = interp(v1.rhw, v2.rhw,t);
		return res;
	}

};



struct PointLight {
	Vec4f pos;
	Vec4f color;
	Vec4f ambient;
};

struct ParallelLight{
	Vec4f direction;
	Vec4f color;
	Vec4f ambient;
};

class VertexIn {
public:

	point_t pos;
	color_t color;
	texcoord_t tc;
	Vec4f normal;
	VertexIn() {}
	VertexIn(point_t pos, color_t color, texcoord_t tc, Vec4f normal)
		:pos(pos), color(color), tc(tc), normal(normal) {}

	VertexIn(const VertexIn &in)
		:pos(in.pos), color(in.color), tc(in.tc), normal(normal) {}
};


class VertexOut {
public:
	//世界变换后的坐标
	point_t posW;
	//投影变换后的坐标
	point_t posP;
	texcoord_t tc;
	Vec4f normal;
	color_t color;
	float oneDivZ;

	VertexOut() {};
	VertexOut(point_t posW, point_t posP, texcoord_t tc, Vec4f normal, color_t color, float w)
		:posW(posW), posP(posP), tc(tc), normal(normal), color(color), oneDivZ(w) {}

	VertexOut& operator= (const VertexOut& rhs)
	{
		if (this == &rhs)
			return *this;
		this->normal = rhs.normal;
		this->posW = rhs.posW;
		this->posP = rhs.posP;
		this->tc = rhs.tc;
		this->color = rhs.color;
		this->oneDivZ = rhs.oneDivZ;
		return *this;
	}

	void onDivZ_init() {
		
		float rhw = oneDivZ;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
		normal.x *=rhw;
		normal.z *= rhw;
		normal.y *= rhw;
	}


	static VertexOut Interp(const VertexOut &v1, const VertexOut &v2, float t) {
		VertexOut res;
		res.posP.x = interp(v1.posP.x, v2.posP.x, t);
		res.posP.y = interp(v1.posP.y, v2.posP.y, t);
		res.posP.z = interp(v1.posP.z, v2.posP.z, t);

		res.posW.x = interp(v1.posW.x, v2.posW.x, t);
		res.posW.y = interp(v1.posW.y, v2.posW.y, t);
		res.posW.z = interp(v1.posW.z, v2.posW.z, t);

		res.normal.x = interp(v1.normal.x, v2.normal.x, t);
		res.normal.y = interp(v1.normal.y, v2.normal.y, t);
		res.normal.z = interp(v1.normal.z, v2.normal.z, t);
		res.normal.w = 0.0f;
		res.normal.normalize();

		res.posP.w = interp(v1.posP.w, v2.posP.w, t);
		res.tc.u = interp(v1.tc.u, v2.tc.u, t);
		res.tc.v = interp(v1.tc.v, v2.tc.v, t);
		res.color.r = interp(v1.color.r, v2.color.r, t);
		res.color.g = interp(v1.color.g, v2.color.g, t);
		res.color.b = interp(v1.color.b, v2.color.b, t);
		res.oneDivZ = interp(v1.oneDivZ, v2.oneDivZ, t);
		return res;
	}
	static VertexOut division(const VertexOut& v1, const VertexOut& v2, float w) {
		float inv = 1.0f / w;
		VertexOut res;
		res.posP.x = (v2.posP.x - v1.posP.x)*inv;
		res.posP.y = (v2.posP.y - v1.posP.y)*inv;
		res.posP.z = (v2.posP.z - v1.posP.z)*inv;
		res.posP.w = (v2.posP.w - v1.posP.w)*inv;

		res.posW.x = (v2.posW.x - v1.posW.x)*inv;
		res.posW.y = (v2.posW.y - v1.posW.y)*inv;
		res.posW.z = (v2.posW.z - v1.posW.z)*inv;
		res.posW.w = (v2.posW.w - v1.posW.w)*inv;

		res.normal.x = (v2.normal.x - v1.normal.x)*inv;
		res.normal.y = (v2.normal.y - v1.normal.y)*inv;
		res.normal.z = (v2.normal.z - v1.normal.z)*inv;

		res.tc.u = (v2.tc.u - v1.tc.u)*inv;
		res.tc.v = (v2.tc.v - v1.tc.v)*inv;

		res.color.r = (v2.color.r - v1.color.r)*inv;
		res.color.g = (v2.color.g - v1.color.g)*inv;
		res.color.b = (v2.color.b - v1.color.b)*inv;

		res.oneDivZ = (v2.oneDivZ - v1.oneDivZ) * inv;
		return res;
	}
	void add(const VertexOut &v) {
		this->posP = v.posP + this->posP;
		this->posW = v.posW + this->posW;
		this->normal = v.normal + this->normal;
		this->tc.u = v.tc.u + this->tc.u;
		this->tc.v = v.tc.v + this->tc.v;
		this->color.r = v.color.r + this->color.r;
		this->color.b = v.color.b + this->color.b;
		this->color.g = v.color.g + this->color.g;
		this->oneDivZ = v.oneDivZ + this->oneDivZ;
	}
};

typedef struct { VertexOut v, v1, v2; } edge_t;
typedef struct { float top, bottom; edge_t left, right; } trapezoid_t;
typedef struct {VertexOut v, step; int x, y, w; } scanline_t;