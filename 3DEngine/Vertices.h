#ifndef __Vertices__
#define __Vertices__


#include "Config.h"
#include "Shaders/Shader.h"


class Vertex
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
};

class Vertices
{
public:
	NEWDEL_DECL
	Vertices();

	enum VerticesType 
	{
		vertices_regular,
		vertices_skin
	};

	virtual ~Vertices();
	
	void FreeDeviceResources();
	void ReInitDeviceResources();
	void UpdateVBO();

	virtual void Allocate(GLuint stride, GLuint count);
	virtual void PutValues(GLvoid * values);

	virtual void Apply(Shader * pShader);
	virtual GLvoid * GetVerticesBuffer()
	{
		return mVerticesBuffer;
	}

	inline VerticesType GetVerticesType()
	{
		return mVerticesType;
	}

public:
	GLvoid * mVerticesBuffer;
	int mCount;
	int mSize;
	GLuint mStride;

protected:
	GLuint mVbo;
	VerticesType mVerticesType;
};


#endif //__Vertices__
