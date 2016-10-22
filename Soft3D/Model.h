
#pragma once
#include <iostream>
#include "Vertex.h"

class Model
{
public:
	int face_num;
	int vert_num;
	Vertex* pVertexs;
	Face* pFaces;

public:
	Model();
	~Model();
	char *Readfile(const char* file) const;
	int LoadMesh(const char *file);
	void set_vertex_normal();
	void face_normal(Vertex &v1, Vertex &v2, Vertex &v3);
};

