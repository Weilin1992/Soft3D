#pragma once
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include "Model.h"
#include "Transform.h"
#include "Vertex.h"

const int RENDER_STATE_WIREFRAME = 1;		// ‰÷»æœﬂøÚ
const int RENDER_STATE_TEXTURE = 2;			// ‰÷»æŒ∆¿Ì
const int RENDER_STATE_COLOR = 4;			// ‰÷»æ—’…´
const int RENDER_STATE_LIGHT = 8;			// ‰÷»æπ‚’’

class Device {
public:
	
	Transform *transform;

	PointLight *pointLight;
	ParallelLight *paraLight;

	Model *model;
	int f_num;
	int v_num;

	int width;
	int height;
	IUINT32 **framebuffer;
	float **zbuffer;

	IUINT32 text[256][256];
	IUINT32 **texture;
	int tex_width;
	int tex_height;
	float max_u;
	float max_v;

	int render_state;
	IUINT32 background;
	IUINT32 foreground;

	Vertex mesh[8];

public:
	Device(int width, int height, IUINT32 *fb);
	~Device();

	void mesh_init();
	void load_model(const char *file);

	void set_render_state(int state);

	void draw_primitive(const Vertex &v1, const Vertex &v2, const Vertex &v3);
	void render_trap(trapezoid_t *trap);
	int trapezoid_init_triangle(trapezoid_t *trap, Vertex *p1, Vertex *p2, Vertex *p3);
	void trapezoid_edge_interp(trapezoid_t *trap, float y);
	void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y);
	void draw_scanline(scanline_t *scanline);

	void draw_line(int x1, int y1, int x2, int y2, IUINT32 c);
	void draw_pixel(int x, int y, IUINT32 color);
	
	void init_texture();
	IUINT32 read_texture(float u, float v);
	void set_texture(void *bits, long pitch, int w, int h);
	void clear(int mode);
	
	void camera_at_zero(float x, float y, float z);

	void transform_Plight(float theta);

	void draw_model(float theta);
	void draw_plane(int a, int b, int c, int d);
	void draw_box(float theta);

};

