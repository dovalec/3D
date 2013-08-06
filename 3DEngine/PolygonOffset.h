#ifndef __PolygonOffset__
#define __PolygonOffset__


#include "Config.h"

class PolygonOffset
{
public:
	NEWDEL_DECL
	PolygonOffset() {
		mFactor = 0.0f;
		mUnits = 0.0f;
	}

	float mFactor;
	float mUnits;
};


#endif //__PolygonOffset__
