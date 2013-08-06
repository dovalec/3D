#include "StdAfx.h"
#include "TransformData.h"

TransformData::TransformData(void)
{
	index = -1;
	parent = -1;

	tx=0.0f;
	ty=0.0f;
	tz=0.0f;

	rx=0.0f;
	ry=0.0f;
	rz=0.0f;
	rw=0.0f;

	sx=0.0f;
	sy=0.0f;
	sz=0.0f;

}

TransformData::~TransformData(void)
{

}
