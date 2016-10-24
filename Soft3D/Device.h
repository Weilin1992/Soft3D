#pragma once
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include "Model.h"
#include "Transform.h"
#include "Vertex.h"
#include "ShaderBase.h"
#include "Shader.h"

const int RENDER_STATE_WIREFRAME = 1;		// äÖÈ¾Ïß¿ò
const int RENDER_STATE_TEXTURE = 2;			// äÖÈ¾ÎÆÀí


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

	Shader * m_shader;

public:
	Device(int width, int height, IUINT32 *fb);
	~Device();

	void mesh_init();
	void load_model(const char *file);
	void set_shader(ShaderBase *shader);
	void set_render_state(int state);

	void draw_primitive(const Vertex &v1, const Vertex &v2, const Vertex &v3);
	void render_trap(trapezoid_t *trap);
	int trapezoid_init_triangle(trapezoid_t *trap, VertexOut *p1, VertexOut *p2, VertexOut *p3);
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

	void face_normal(Vertex &v1, Vertex &v2, Vertex &v3) {

		Vec4f& p1 = v1.pos;
		Vec4f& p2 = v2.pos;
		Vec4f& p3 = v3.pos;

		Vec4f edge1, edge2, pn;

		edge1 = p2 - p1;
		edge2 = p3 - p2;


		pn = edge1.cross(edge2);
		pn.normalize();
		pn.w = 0.0f;
		v1.normal = pn;
		v2.normal = pn;
		v3.normal = pn;
	}

	void set_paralight(ParallelLight *p) {
		m_shader->SetParallelLight(p);
	}
	void set_pointlight(PointLight *p) {
		m_shader->SetPointLight(p);
	}


};

