#ifndef __VerticesColors__
#define __VerticesColors__

#include "Config.h"

class VerticesColors
{

public:
	float * mColorsBuffer;
	int mCount;

	VerticesColors();
	virtual void Allocate(int count);
	virtual void PutValues(float * values);


};

#endif //__VerticesColors__

