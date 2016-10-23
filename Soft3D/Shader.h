#pragma once
#include "ShaderBase.h"
class Shader :
	public ShaderBase
{
public:
	Shader();
	~Shader();
private:
	Mat44f worldViewProj; //world view matrix
	IUINT32 **texture; //texture
	Mat44f world;
	Mat44f worldInvTranspose; //normal
	PointLight * pointLight;
	ParallelLight *paraLight;
	Vec4f eyePos;
public:
	void SetWorldViewProj(const Mat44f& worldViewP);
	void SetTexture(IUINT32 **text);
	void SetWorld(const Mat44f& w);
	void SetEyePos(const Vec4f& eyeP);
	void SetPointLight(PointLight * p);
	void SetParallelLight(ParallelLight *p);
public:
	VertexOut VS(const Vertex& vin);
	Vec4f PS(VertexOut& pin);

};

