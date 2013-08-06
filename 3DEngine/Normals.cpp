#include "Normals.h"

Normals::Normals()
{

}

void Normals::Allocate(int count)
{
	mNormalBuffer = (float*)malloc(count * 3 * sizeof(float));
	mCount = count;
}

void Normals::PutValues(float * values)
{
	memcpy(mNormalBuffer, values, mCount * 3 * sizeof(float));
}

