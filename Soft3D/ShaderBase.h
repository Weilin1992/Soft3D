#pragma once
#include "Vertex.h"

class ShaderBase {
public:
	ShaderBase();
	virtual ~ShaderBase();

public:
	virtual VertexOut VS(const Vertex& vin) = 0;//vertex shader
	virtual IUINT32 PS(VertexOut& pin) = 0;//pixel shader

};