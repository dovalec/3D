#include "VerticesColors.h"

VerticesColors::VerticesColors()
{

}

void VerticesColors::Allocate(int count)
{
	mColorsBuffer = (float*)malloc(count * 4 * sizeof(float));
	mCount = count;
}

void VerticesColors::PutValues(float * values)
{
	memcpy(mColorsBuffer, values, mCount * 4 * sizeof(float));
}

