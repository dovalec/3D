#ifndef __VerticesSkin__
#define __VerticesSkin__


#include "Config.h"
#include "Vertices.h"


class VertexSkin
{
public:
	NEWDEL_DECL
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

class VerticesSkin : public Vertices
{
public:
	NEWDEL_DECL
	VerticesSkin();
	virtual void Apply(Shader * pShader);

};


#endif //__VerticesSkin__
