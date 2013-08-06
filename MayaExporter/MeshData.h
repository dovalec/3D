#pragma once

class Vertex
{
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float u;
	float v;
};



class VertexSkin
{
public:
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float w0;
	float w1;
	float w2;
	float w3;

	char t0;
	char t1;
	char t2;
	char t3;
};


class MeshData
{
public:
	MeshData(void);
	~MeshData(void);

public:
	string name;

	int layer;
	int glsl_shader;
	int alpha_mode;
	int display_mode;

	int use_depth;
	int use_back_face_culling;
	int use_occlusion_culling;

	int index;
	int transformIndex;
	int materialIndex;
	float radius;
	
	VertexSkin * pVertices;
	int verticesCount;

	vector<unsigned short*> mSubMeshIndices;
	vector<int> mSubMeshIndicesCount;

	map< int ,int> mConvUV;

};
