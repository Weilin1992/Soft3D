#include "Shader.h"



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

void Shader::SetTexture(IUINT32 ** text)
{
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

Vec4f Shader::PS(VertexOut & pin)
{
	return Vec4f();
}
