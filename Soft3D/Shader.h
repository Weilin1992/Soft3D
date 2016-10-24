#pragma once
#include "ShaderBase.h"
#include <math.h>
class Shader :
	public ShaderBase
{
public:
	Shader();
	~Shader();
private:
	Mat44f worldViewProj; //world view matrix
	
	Mat44f world;
	Mat44f worldInvTranspose; //normal
	PointLight * pointLight;
	ParallelLight *paraLight;
	Vec4f eyePos;


	float kd = 1;
	float ks = 1;
	float ns = 64;

	int tex_width;
	int tex_height;
	float max_u;
	float max_v;
	IUINT32 **texture; //texture
public:
	void SetWorldViewProj(const Mat44f& worldViewP);
	void SetTexture(IUINT32 **text,int w, int h);
	void SetWorld(const Mat44f& w);
	void SetEyePos(const Vec4f& eyeP);
	void SetPointLight(PointLight * p);
	void SetParallelLight(ParallelLight *p);
	Vec4f read_texture(float u, float v)
	{

			int x, y;
			u = u*max_u;
			v = v*max_v;
			x = (int)(u + 0.5f);
			y = (int)(v + 0.5f);
			x = CMID(x, 0, tex_width - 1);
			y = CMID(y, 0, tex_height - 1);
			IUINT32 cc = texture[y][x];
			Vec4f res;
			float inv = (float)1 / 255;
			res = { (cc >> 16) * inv, (cc >> 8 & 0xff) * inv, (cc & 0xff) * inv,1.0};
			return res;
	}


public:
	VertexOut VS(const Vertex& vin);
	IUINT32 PS(VertexOut& pin);

};

