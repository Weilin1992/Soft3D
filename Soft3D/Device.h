#pragma once
#include "IMath.h"
#include <cstdlib>
#include <assert.h>

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
	
	~Device() {
		delete transform;
	}

};