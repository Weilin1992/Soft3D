#include "Device.h"

Device::Device(int width, int height, IUINT32 * fb)
{
	model = new Model();
	this->width = width;
	this->height = height;
	transform = new Transform();
	framebuffer = (IUINT32**)malloc(height*sizeof(IUINT32*));

	for (int y = 0; y < height; y++) {
		framebuffer[y] = fb + y * width;
	}
	zbuffer = (float**)malloc(height*sizeof(float*));
	for (int y = 0; y < height; y++) {
		zbuffer[y] = (float*)malloc(width * sizeof(float));
		memset(zbuffer[y], 0.0f, width*sizeof(float));
	}

	texture = (IUINT32**)malloc(height*sizeof(IUINT32*));

	background = 0xc0c0c0;
	foreground = 0;

	transform->init(width, height, 3.1415926 * 0.5f);
	render_state = RENDER_STATE_WIREFRAME;
	mesh_init();
}

void Device::set_render_state(int state)
{
	render_state = state;
}

void Device::load_model(const char * file)
{
	model->LoadMesh(file);
	f_num = model->face_num;
	v_num = model->vert_num;
}

void Device::draw_primitive(const Vertex & v1, const Vertex & v2, const Vertex & v3)
{

	point_t p1, p2, p3, c1, c2, c3;
	Vec4f n1, n2, n3;

	//按照transform变化
	c1 = transform->apply(v1.pos);
	c2 = transform->apply(v2.pos);
	c3 = transform->apply(v3.pos);

	if (transform->check_cvv(c1) != 0) return;
	if (transform->check_cvv(c2) != 0) return;
	if (transform->check_cvv(c3) != 0) return;

	p1 = transform->homogenize(c1);
	p2 = transform->homogenize(c2);
	p3 = transform->homogenize(c3);

	n1 = transform->apply(v1.normal);
	n2 = transform->apply(v2.normal);
	n3 = transform->apply(v3.normal);


	//
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		Vertex t1 = v1, t2 = v2, t3 = v3;
		int n;
		trapezoid_t traps[2];


		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;

		t1.normal = n1;
		t2.normal = n2;
		t3.normal = n3;

		t1.rhw_init();
		t2.rhw_init();
		t3.rhw_init();

		n = trapezoid_init_triangle(traps, &t1, &t2, &t3);

		if (n >= 1) render_trap(&traps[0]);
		if (n >= 2) render_trap(&traps[1]);
	}
	if (render_state &RENDER_STATE_WIREFRAME) {
		draw_line((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, foreground);
		draw_line((int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, foreground);
		draw_line((int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, foreground);
	}
}

int Device::trapezoid_init_triangle(trapezoid_t * trap, Vertex * p1, Vertex * p2, Vertex * p3)
{
	{
		Vertex *p = new Vertex();
		float k, x;

		if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
		if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
		if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
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

		if (x <= p3->pos.x) {		// triangle left
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p2;
			trap[0].right.v1 = *p1;
			trap[0].right.v2 = *p3;
			trap[1].left.v1 = *p2;
			trap[1].left.v2 = *p3;
			trap[1].right.v1 = *p1;
			trap[1].right.v2 = *p3;
		}
		else {					// triangle right
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p3;
			trap[0].right.v1 = *p1;
			trap[0].right.v2 = *p2;
			trap[1].left.v1 = *p1;
			trap[1].left.v2 = *p3;
			trap[1].right.v1 = *p2;
			trap[1].right.v2 = *p3;
		}
		return 2;
	}
}

void Device::trapezoid_edge_interp(trapezoid_t * trap, float y)
{
	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
	float t1 = (y - trap->left.v1.pos.y) / s1;
	float t2 = (y - trap->right.v1.pos.y) / s2;
	trap->left.v = Vertex::Interp(trap->left.v1, trap->left.v2, t1);
	trap->right.v = Vertex::Interp(trap->right.v1, trap->right.v2, t2);
}

void Device::trapezoid_init_scan_line(const trapezoid_t * trap, scanline_t * scanline, int y)
{
	float width = trap->right.v.pos.x - trap->left.v.pos.x;
	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
	scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
	scanline->y = y;
	scanline->v = trap->left.v;
	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
	scanline->step = Vertex::division(trap->left.v, trap->right.v, width);
}

void Device::draw_scanline(scanline_t * scanline)
{
	IUINT32 *fb = framebuffer[scanline->y];
	float *zb = zbuffer[scanline->y];
	int x = scanline->x;
	int w = scanline->w;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width) {
			float rhw = scanline->v.rhw;
			if (rhw >= zb[x]) {
				float ww = 1.0f / rhw;
				zb[x] = rhw;
				if (render_state & RENDER_STATE_COLOR) {
					float r = scanline->v.color.r * ww;
					float g = scanline->v.color.g * ww;
					float b = scanline->v.color.b * ww;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);
					fb[x] = (R << 16) | (G << 8) | (B);
					IUINT32 tmp = fb[x];
				}
				if (render_state & RENDER_STATE_TEXTURE) {
					float u = scanline->v.tc.u * ww;
					float v = scanline->v.tc.v * ww;
					IUINT32 cc = read_texture(u, v);

					Vec4f lightdir = pointLight->pos - scanline->v.pos;
					lightdir.normalize();
					scanline->v.normal.normalize();
					float diff = scanline->v.normal.dot(lightdir);

					diff = diff > 0 ? diff : 0;


					float inv = (float)1 / 255;
					color_t tex_color = { (cc >> 16) * inv, (cc >> 8 & 0xff) * inv, (cc & 0xff) * inv };
					color_t ambient = { 1.0f, 1.0f, 1.0f };
					float intensity = 0.3;

					// 环境光的影响
					ambient.r *= intensity * tex_color.r;
					ambient.g *= intensity * tex_color.g;
					ambient.b *= intensity * tex_color.b;

					color_t diffuse = { 0.f, 0.f, 0.f };
					if (diff > 0) {
						diffuse.r = tex_color.r * diff;
						diffuse.g = tex_color.g * diff;
						diffuse.b = tex_color.b * diff;
					}
					ambient.r += diffuse.r;
					ambient.g += diffuse.g;
					ambient.b += diffuse.b;
					color_t res;
					res = ambient;
					res.r = res.r > 1.f ? 1.f : res.r;
					res.g = res.g > 1.f ? 1.f : res.g;
					res.b = res.b > 1.f ? 1.f : res.b;
					cc = (int(res.r * 255) << 16 | int(res.g * 255) << 8 | int(res.b * 255));

					fb[x] = cc;
				}



			}
		}
		scanline->v.add(scanline->step);
		if (x >= width) break;
	}

}

void Device::draw_line(int x1, int y1, int x2, int y2, IUINT32 c)
{
	int x, y, rem = 0;
	if (x1 == x2&&y1 == y2) {
		draw_pixel(x1, y2, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc)
			draw_pixel(x2, y, c);
		draw_pixel(x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc)
			draw_pixel(x, y1, c);
		draw_pixel(x2, y1, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx > dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				draw_pixel(x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					draw_pixel(x, y, c);
				}
			}
			draw_pixel(x2, y2, c);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				draw_pixel(x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					draw_pixel(x, y, c);
				}
			}
			draw_pixel(x2, y2, c);
		}
	}
}

void Device::draw_pixel(int x, int y, IUINT32 color)
{
	if (((IUINT32)x) < (IUINT32)width && ((IUINT32)y) < (IUINT32)height) {
		framebuffer[y][x] = color;
	}
}

void Device::render_trap(trapezoid_t * trap)
{
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
		if (j >= height) break;
	}
}

IUINT32 Device::read_texture(float u, float v)
{
	{
		int x, y;
		u = u*max_u;
		v = v*max_v;
		x = (int)(u + 0.5f);
		y = (int)(v + 0.5f);
		x = CMID(x, 0, tex_width - 1);
		y = CMID(y, 0, tex_height - 1);
		return texture[y][x];
	}
}

void Device::set_texture(void * bits, long pitch, int w, int h)
{
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

void Device::clear(int mode)
{
	int y, x;
	for (y = 0; y < height; y++) {
		IUINT32 *dst = framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = background;
		for (x = width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < height; y++) {
		float *dst = zbuffer[y];
		for (x = width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

void Device::init_texture()
{

	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			text[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}
	set_texture(text, 256 * 4, 256, 256);
}

void Device::camera_at_zero(float x, float y, float z)
{
	point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 1, 0, 1 };
	transform->set_look_at(eye, at, up);
	transform->update();
}

void Device::draw_model(float theta)
{
	Mat44f m;
	m.SetRotate(0.0f, 1.0f, 0.0f, theta);
	transform->world = m;
	transform->update();
	Vertex *v = model->pVertexs;
	Face *face = model->pFaces;
	for (int i = 0; i < f_num; i++) {
		int i1 = face[i].i1;
		int i2 = face[i].i2;
		int i3 = face[i].i3;

		Vertex v1 = v[i1];
		Vertex v2 = v[i2];
		Vertex v3 = v[i3];
		v1.tc = { 0,0 };
		v2.tc = { 0,0 };
		v3.tc = { 0,0 };
		draw_primitive(v1, v2, v3);
	}
}

void Device::draw_plane(int a, int b, int c, int d)
{
	Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	draw_primitive(p1, p2, p3);
	draw_primitive(p3, p4, p1);
}

void Device::draw_box(float theta)
{
	Mat44f m;
	m.SetRotate(-1, -0.5, 1, theta);
	transform->world = m;
	transform->update();
	draw_plane(0, 1, 2, 3);
	draw_plane(4, 5, 6, 7);
	draw_plane(0, 4, 5, 1);
	draw_plane(1, 5, 6, 2);
	draw_plane(2, 6, 7, 3);
	draw_plane(3, 7, 4, 0);
}

void Device::transform_Plight(float theta)
{

}


Device::~Device()
{
	delete transform;
	delete model;
	free(zbuffer);
	free(framebuffer);
	free(texture);
}

void Device::mesh_init()
{
	{
		mesh[0].pos = { 1,-1,1,1 };
		mesh[0].tc = { 0,0 };
		mesh[0].color = { 1.0f, 0.2f, 0.2f };
		mesh[0].rhw = 1;

		mesh[1].pos = { -1, -1,  1, 1 };
		mesh[1].tc = { 0,1 };
		mesh[1].color = { 0.2f, 1.0f, 0.2f };
		mesh[1].rhw = 1;

		mesh[2].pos = { -1,  1,  1, 1 };
		mesh[2].tc = { 1,1 };
		mesh[2].color = { 0.2f, 0.2f, 1.0f };
		mesh[2].rhw = 1;

		mesh[3].pos = { 1,  1,  1, 1 };
		mesh[3].tc = { 1, 0 };
		mesh[3].color = { 1.0f, 0.2f, 1.0f };
		mesh[3].rhw = 1;

		mesh[4].pos = { 1, -1, -1, 1 };
		mesh[4].tc = { 0, 0 };
		mesh[4].color = { 1.0f, 1.0f, 0.2f };
		mesh[4].rhw = 1;

		mesh[5].pos = { -1, -1, -1, 1 };
		mesh[5].tc = { 1,0 };
		mesh[5].color = { 0.2f, 1.0f, 1.0f };
		mesh[5].rhw = 1;

		mesh[6].pos = { -1,  1, -1, 1 };
		mesh[6].tc = { 1,1 };
		mesh[6].color = { 1.0f, 0.3f, 0.3f };
		mesh[6].rhw = 1;

		mesh[7].pos = { 1,1,-1,1 };
		mesh[7].tc = { 1,0 };
		mesh[7].color = { 0.2f, 1.0f, 0.3f };
		mesh[7].rhw = 1;


	}
}
