#ifndef __TextureCoords__
#define __TextureCoords__



#include "Config.h"

class TextureCoords
{

public:
	float * mUVBuffer;
	int mCount;
	int mSize;

	TextureCoords();

	virtual void Allocate(int count);
	virtual void PutValues(float * values);
	virtual void Apply();

protected:
	GLuint mVbo;

};

#endif //__TextureCoords__
