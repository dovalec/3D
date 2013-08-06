#ifndef __Indices__
#define __Indices__

#include "Config.h"
#include "TrianglesMode.h"

#define MAX_SUB_MESH 32
class Indices
{
public:
	NEWDEL_DECL
	GLushort* mIndicesBuffer;
	int mCount;
	int mSize;
	int mSubMeshCount;
	int mSubMeshSizes[MAX_SUB_MESH];

	GLuint mVbo;


	TrianglesMode mTriangleMode;

	Indices();
	virtual ~Indices();

	void FreeDeviceResources();
	void ReInitDeviceResources();
	virtual void Apply();
	virtual void Allocate(int count);
	virtual void PutValues(GLushort * values);
	virtual void PutValuesPartial(GLushort * values, int place, int size);

public:
};

#endif //__Indices__
