#ifndef __Color__
#define __Color__

#include "Config.h"

class Color
{
public:
	NEWDEL_DECL
	float RGBA[4];
	Color(float r, float g, float b, float a);
	Color();
};

#endif //__Color__

