#pragma once
#include "IMath.h"

typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
typedef Vec4f point_t;

class Vertex {
private:
	point_t pos;
	texcoord_t tc;
	color_t color;
	float rhw;

public:
	Vertex(point_t pos,color_t color, texcoord_t tc):
	pos(pos),color(color),tc(tc)
	{
		rhw = 10.f/ pos.w;
		rhw_init();
	}

	void rhw_init() {
		tc.u *= rhw;
		tc.v *= rhw;
		color.r *= rhw;
		color.g *= rhw;
		color.b *= rhw;
	}

	Vertex interp(const Vertex& v, float t) {

	}
	
	Vertex division(const Vertex& v1, const Vertex& v2, float w) {

	}

	Vertex add(const Vertex &v) {

	}
	

};

