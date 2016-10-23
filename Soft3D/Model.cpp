#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
	delete[] pFaces;
	delete[] pVertexs;

	 
}

char * Model::Readfile(const char * file) const 
{
	FILE *pFile;
	int err = fopen_s(&pFile, file, "rb");
	if (err == 1)  //if(err == NULL)
	{
		printf("ERROR！");

	}
	if (!pFile) {
		return NULL;
	}

	char *pBuf;
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	pBuf = new char[len + 1];
	rewind(pFile);
	fread(pBuf, 1, len, pFile);
	pBuf[len] = '\0';
	fclose(pFile);
	return pBuf;
}

int Model::LoadMesh(const char * file)
{
	char* pFile;
	pFile = Readfile(file);
	if (!pFile) {
		return 0;
	}

	char* pSrc;
	pSrc = pFile;

	int i, vc, fc;

	// 计算顶点和面的个数
	i = 0, vc = 0, fc = 0;
	char line[1024];
	memset(line, 0, 1024);
	for (; *pSrc != '\0';) {
		if (*pSrc == '\n') {
			if (line[0] == 'v') {
				++vc;
			}
			else if (line[0] == 'f') {
				++fc;
			}

			i = 0;
			memset(line, 0, 1024);
		}
		else {
			if (i > 1024) {
				delete pFile;
				return 0;
			}
			line[i++] = *pSrc;
		}
		++pSrc;
	}
	if (vc == 0 || fc == 0) {
		delete pFile;
		return 0;
	}

	vert_num = vc; face_num = fc;

	if (pVertexs != nullptr) delete[] pVertexs;
	if (pFaces != nullptr) delete[] pFaces;

	pVertexs = new Vertex[vc];
	pFaces = new Face[fc];

	pSrc = pFile;

	// 读取数据
	i = 0, vc = 0, fc = 0;
	memset(line, 0, 1024);
	for (; *pSrc != '\0';) {
		if (*pSrc == '\n') {
			if (line[0] == 'v') {
				float x, y, z;
				sscanf_s(line, "v %f %f %f", &x, &y, &z);
				pVertexs[vc].pos = { x, y, z, 1.f };
				pVertexs[vc].color = { 0.f, 0.f, 0.f };
				pVertexs[vc].tc = { 0.f, 1.f };
				pVertexs[vc].rhw = 1.f;
				vc++;
			}
			else if (line[0] == 'f') {
				int p1, p2, p3;
				sscanf_s(line, "f %d %d %d", &p1, &p2, &p3);

				pFaces[fc++] = { p1 - 1, p2 - 1, p3 - 1 };
			}

			i = 0;
			memset(line, 0, 1024);
		}
		else {
			line[i++] = *pSrc;
		}
		++pSrc;
	}
	set_vertex_normal();
		
	delete pFile;

	return 1;

}

void Model::set_vertex_normal() {


	for (int i = 0; i < face_num; i++) {
		face_normal(pVertexs[pFaces[i].i1], pVertexs[pFaces[i].i2], pVertexs[pFaces[i].i3]);
	}

	for (int i = 0; i < vert_num; i++) {
		pVertexs[i].normal.normalize();
	}
}

void Model::face_normal(Vertex &v1, Vertex &v2, Vertex &v3) {

	Vec4f& p1 = v1.pos;
	Vec4f& p2 = v2.pos;
	Vec4f& p3 = v3.pos;

	Vec4f edge1, edge2, pn;

	edge1 = p2 - p1;
	edge2 = p3 - p2;

	pn = edge1.cross(edge2);
	pn.normalize();
	pn.w = 0.0f;
	v1.normal = v1.normal + pn;
	v2.normal = v2.normal + pn;
	v3.normal = v3.normal + pn;
}
