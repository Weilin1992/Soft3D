#pragma once
#include "IMath.h"

typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
typedef Vec4f point_t;

class Vertex {
public:
	point_t pos;
	texcoord_t tc;
	color_t color;
	float rhw;

public:
	Vertex() {}
	Vertex(point_t pos,color_t color, texcoord_t tc):
	pos(pos),color(color),tc(tc)
	{
		rhw = 10.f/ pos.w;
		rhw_init();
	}

	void rhw_init() {
		rhw = 1.0f / pos.w;
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	}

	Vertex interp(const Vertex& v, float t) {

	}
	
	static Vertex division(const Vertex& v1, const Vertex& v2, float w) {
		float inv = 1.0f / w;
		Vertex res;
		res.pos.x = (v2.pos.x - v1.pos.x)*inv;
		res.pos.y = (v2.pos.y - v1.pos.y)*inv;
		res.pos.z = (v2.pos.z - v1.pos.z)*inv;
		res.pos.w = (v2.pos.w - v1.pos.w)*inv;

		res.tc.u = (v2.tc.u - v1.tc.v)*inv;
		res.tc.v = (v2.tc.u - v1.tc.v)*inv;

		res.color.r = (v2.color.r - v1.color.r)*inv;
		res.color.g = (v2.color.g - v1.color.g)*inv;
		res.color.b = (v2.color.b - v1.color.b)*inv;

		res.rhw = (v2.rhw - v1.rhw) * inv;
		return res;
	}

	Vertex add(const Vertex &v) {

	}
	
	static Vertex Interp(const Vertex &v1,const Vertex &v2,float t) {

	}

};



typedef struct { Vertex v, v1, v2; } edge_t;
typedef struct { float top, bottom; edge_t left, right; } trapezoid_t;
typedef struct { Vertex v, step; int x, y, w; } scanline_t;

