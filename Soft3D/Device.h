#pragma once
#include "IMath.h"
#include <cstdlib>
#include <assert.h>
#include "Vertex.h"
const int RENDER_STATE_WIREFRAME = 1;		// äÖÈ¾Ïß¿ò
const int RENDER_STATE_TEXTURE = 2;			// äÖÈ¾ÎÆÀí
const int RENDER_STATE_COLOR = 3;			// äÖÈ¾ÑÕÉ«
class Device {
private:
	Transform *transform;
	int width;
	int height;
	IUINT32 **framebuffer;
	float **zbuffer;
	IUINT32 **texture;
	int tex_width;
	int tex_height;
	float max_u;
	float max_v;
	int render_state;
	IUINT32 background;
	IUINT32 foreground;

public:
	Device(int width,int height,void *fb) {
		this->width = width;
		this->height = height;
		transform = new Transform();
		int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
		char *ptr = (char*)malloc(need + 64);
		char *framebuf, *zbuf;
		int j;
		assert(ptr);
		framebuffer = (IUINT32**)ptr;
		



	}
	
	void set_render_state(int state) {
		render_state = state;
	}

	~Device() {
		delete transform;
	}

	void draw_primitive(const Vertex &v1,const Vertex &v2, const Vertex &v3) {

		point_t p1, p2, p3, c1, c2, c3;

		//°´ÕÕtransform±ä»¯
		c1 = transform->apply(v1.pos);
		c2 = transform->apply(v2.pos);
		c3 = transform->apply(v3.pos);
		
		if (transform->check_cvv(c1) != 0) return;
		if (transform->check_cvv(c2) != 0) return;
		if (transform->check_cvv(c3) != 0) return;

		p1 = transform->homogenize(c1);
		p2 = transform->homogenize(c2);
		p3 = transform->homogenize(c3);

		//
		if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
			Vertex t1 = v1, t2 = v2, t3 = v3;
			int n;
			trapezoid_t traps[2];

			t2.pos = p2;
			t1.pos = p1;
			t3.pos = p3;
			t1.pos.w = c1.w;
			t2.pos.w = c2.w;
			t3.pos.w = c3.w;

			t1.rhw_init();
			t2.rhw_init();
			t3.rhw_init();

			
			
		}


	}

	int trapezoid_init_triangle(trapezoid_t *trap, const Vertex *p1, const Vertex *p2, const Vertex *p3) {
		const Vertex *p;
		float k, x;

		if (p1->pos.y > p2->pos.y) p = p1; p1 = p2; p2 = p;
		if (p1->pos.y > p3->pos.y) p = p1; p1 = p3; p3 = p;
		if (p2->pos.y > p3->pos.y) p = p2; p2 = p3; p3 = p;
		if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
		if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

		if (p1->pos.y == p2->pos.y) {
			if (p1->pos.x > p2->pos.x) p = p1, p1 = p2, p2 = p;
			trap[0].top = p1->pos.y;
			trap[0].bottom = p3->pos.y;
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p3;
			trap[0].right.v1 = *p2;
			trap[0].right.v2 = *p3;
			return (trap[0].top < trap[0].bottom) ? 1 : 0;
		}

		if (p2->pos.y == p3->pos.y) {
			if (p2->pos.x > p3->pos.x) p = p2, p2 = p3, p3 = p;
			trap[0].top = p1->pos.y;
			trap[0].bottom = p2->pos.y;
			trap[0].left.v1 = *p1;
			trap[0].right.v1 = *p1;
			trap[0].left.v2 = *p2;
			trap[0].left.v2 = *p3;
			return (trap[0].top < trap[0].bottom) ? 1 : 0;
		}

		trap[0].top = p1->pos.y;
		trap[0].bottom = p2->pos.y;
		trap[1].top = p2->pos.y;
		trap[1].bottom = p3->pos.y;

		k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
		x = p1->pos.x + (p2->pos.x - p1->pos.x) * k; 



		return 2;
	}

	void trapezoid_edge_interp(trapezoid_t *trap, float y) {
		float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
		float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
		float t1 = (y - trap->left.v1.pos.y) / s1;
		float t2 = (y - trap->right.v1.pos.y) / s2;
		trap->left.v = Vertex::Interp(trap->left.v1, trap->left.v2, t1);
		trap->right.v = Vertex::Interp(trap->right.v1, trap->right.v2, t2);
		
	}

	void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
		float width = trap->right.v.pos.x - trap->left.v.pos.x;
		scanline->x = (int)(trap->left.v.pos.x + 0.5f);
		scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
		scanline->y = y;
		scanline->v = trap->left.v;
		if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
		scanline->step = Vertex::division(trap->left.v, trap->right.v, width);
	}
	void draw_scanline(scanline_t *scanline) {
		IUINT32 *fb = framebuffer[scanline->y];
		float *zb = zbuffer[scanline->y];
		int x = scanline->x;
		int w = scanline->w;
		for (; w > 0; x++, w--) {
			if (x >= 0 && x < width) {
				float rhw = scanline->v.rhw;
				if (rhw >= zb[x]) {
					float w = 1.0f / rhw;
					zb[x] = rhw;
					if (render_state & RENDER_STATE_COLOR) {
						float r = scanline->v.color.r *w;
						float g = scanline->v.color.g * w;
						float b = scanline->v.color.b * w;
						int R = (int)(r * 255.0f);
						int G = (int)(g * 255.0f);
						int B = (int)(b * 255.0f);
						R = CMID(R, 0, 255);
						G = CMID(G, 0, 255);
						B = CMID(B, 0, 255);
						fb[x] = (R << 16 | G << 8 | B);
					}
					if (render_state & RENDER_STATE_TEXTURE) {
						float u = scanline->v.tc.u * w;
						float v = scanline->v.tc.v * w;
						IUINT32 cc = read_texture(u, v);
						fb[x] = cc;
					}
				}
			}
			scanline->v.add(scanline->step);
			if (x >= width) break;
		}

	}

	void render_trap(trapezoid_t *trap) {
		scanline_t scanline;
		int j, top, bottom;
		top = (int)(trap->top + 0.5f);
		bottom = (int)(trap->bottom + 0.5f);
		for (j = top; j < bottom; j++) {
			if (j >= 0 && j < height) {
				trapezoid_edge_interp(trap, (float)j + 0.5f);
				trapezoid_init_scan_line(trap, &scanline, j);
				draw_scanline(&scanline);

			}
		}

	}
	IUINT32 read_texture(float u, float v) {
		int x, y;
		u = u*max_u;
		v = v*max_v;
		x = (int)(u + 0.5f);
		y = (int)(v + 0.5f);
		x = CMID(x, 0, tex_width - 1);
		y = CMID(y, 0, tex_height - 1);
		return texture[y][x];

	}

	void set_texture(void *bits, long pitch, int w, int h) {
		char *ptr = (char*)bits;
		int j;
		assert(w < 1024 && h <= 1024);
		for (j = 0; j < h; ptr += pitch, j++)
			texture[j] = (IUINT32*)ptr;
		tex_width = w;
		tex_height = h;
		max_u = (float)(w - 1);
		max_v = (float)(h - 1);
	}


};