
#include "IMath.h"
#include <iostream>
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
		res.pos.w = interp(v1.pos.w, v2.pos.w, t);
		res.tc.u = interp(v1.tc.u, v2.tc.u, t);
		res.tc.v = interp(v1.tc.v, v2.tc.v, t);
		res.color.r = interp(v1.color.r, v2.color.r, t);
		res.color.g = interp(v1.color.g, v2.color.g, t);
		res.color.b = interp(v1.color.b, v2.color.b, t);
		res.rhw = interp(v1.rhw, v2.rhw,t);
		//std::cout << res.rhw;
		return res;
	}

};






typedef struct { Vertex v, v1, v2; } edge_t;
typedef struct { float top, bottom; edge_t left, right; } trapezoid_t;
typedef struct { Vertex v, step; int x, y, w; } scanline_t;

