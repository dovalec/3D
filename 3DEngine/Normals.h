#ifndef __Normals__
#define __Normals__


#include "Config.h"

class Normals
{

public:
	float * mNormalBuffer;
	int mCount;

	Normals();

	Normals(float * values);

	virtual void Allocate(int count);
	virtual void PutValues(float * values);


};

#endif //__Normals__
