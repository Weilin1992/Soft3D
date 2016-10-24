#include "Shader.h"
#include <iostream>


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::SetWorldViewProj(const Mat44f & worldViewP)
{
	worldViewProj = worldViewP;
}

void Shader::SetTexture(IUINT32 ** text, int w, int h)
{
	tex_width = w;
	tex_height = h;
	max_u = (float)(w - 1);
	max_v = (float)(h - 1);
	texture = text;
}

void Shader::SetWorld(const Mat44f & w)
{
	world = w;
}

void Shader::SetEyePos(const Vec4f & eyeP)
{
	eyePos = eyeP;
}

void Shader::SetPointLight(PointLight * p)
{
	pointLight = p;
}

void Shader::SetParallelLight(ParallelLight * p)
{
	paraLight = p;
}

VertexOut Shader::VS(const Vertex & vin)
{
	VertexOut out;
	out.posP = mul(vin.pos, worldViewProj);
	out.posW = mul(vin.pos, world);
	out.normal = mul(vin.normal, world);// I don't have enough time,don't do any scaling on the model so that we can use the same matrix
	out.color = vin.color;
	out.tc = vin.tc;
	return out;
}

IUINT32 Shader::PS(VertexOut & pin)
{
	IUINT32 res;
	Vec4f N = pin.normal;
	N.normalize();
	Vec4f V = (eyePos - pin.posW).normalize();
	Vec4f L = paraLight->direction.normalize();
	Vec4f N2 = N * 2;
	Vec4f R = N*fmax(N2.dot(L), 0) - L;
	R.normalize();


	Vec4f ambient = paraLight->ambient;
	Vec4f &lightcolor = paraLight->color;
	Vec4f texColor = read_texture(pin.tc.u, pin.tc.v);
	//texColor ={(float) 0.0666666701, (float)0.933333397,(float)0.933333397,1.0f};
	float diff = N.dot(L);
	
	ambient.x *= texColor.x;
	ambient.y *= texColor.y;
	ambient.z *= texColor.z;

	Vec4f diffuse = (ambient + texColor*fmax(N.dot(L), 0));
	Vec4f specular = (texColor*pow(fmax(V.dot(R), 0), 32)) * 10;
	


	Vec4f color = diffuse + specular;

	color.x = color.x > 1.f ? 1.f : color.x;
	color.y = color.y > 1.f ? 1.f : color.y;
	color.z = color.z > 1.f ? 1.f : color.z;

	res = (int(color.x * 255) << 16 | int(color.y * 255) << 8 | int(color.z * 255));
	return res;
}

