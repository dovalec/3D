#include "Color.h"

NEWDEL_IMPL(Color)
Color::Color(float r, float g, float b, float a)
{
	RGBA[0] = r;
	RGBA[1] = g;
	RGBA[2] = b;
	RGBA[3] = a;

}

Color::Color()
{
	RGBA[0] = 0.0f;
	RGBA[1] = 0.0f;
	RGBA[2] = 0.0f;
	RGBA[3] = 0.0f;

}
