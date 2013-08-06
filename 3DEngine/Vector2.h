#ifndef __Vector2__
#define __Vector2__


#include "Config.h"

class Vector2
{
public:
	NEWDEL_DECL

	float X;
	float Y;

	Vector2();

	Vector2(float x, float y);

	static Vector2 * Add(Vector2 * a, Vector2 * b, Vector2 * ret);

	static Vector2 * Add(Vector2 * a, float s, Vector2 * ret);

	static Vector2 * Sub(Vector2 * a, Vector2 * b, Vector2 * ret);

	static Vector2 * Mul(Vector2 * a, Vector2 * b, Vector2 * ret);

	static Vector2 * Mul(Vector2 * a, float s, Vector2 * ret);

	static float LengthSquared(Vector2 * a);

	static float Length(Vector2 * a);

	static Vector2 * Normalize(Vector2 * a, Vector2 * ret);

	virtual void Print();
};

#endif //__Vector2__
