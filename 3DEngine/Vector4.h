#ifndef __Vector4__
#define __Vector4__

#include "Config.h"

class Vector4
{
public:
	NEWDEL_DECL

	float X;
	float Y;
	float Z;
	float W;

	Vector4();

	Vector4(float x, float y, float z, float w);

	static Vector4 * Add(Vector4 * a, Vector4 * b, Vector4 * ret);

	static Vector4 * Add(Vector4 * a, float s, Vector4 * ret);

	static Vector4 * Sub(Vector4 * a, Vector4 * b, Vector4 * ret);

	static Vector4 * Mul(Vector4 * a, Vector4 * b, Vector4 * ret);

	static Vector4 * Mul(Vector4 * a, float s, Vector4 * ret);

	static Vector4 * CrossProduct(Vector4 * a, Vector4 * b, Vector4 * ret);

	static float LengthSquared(Vector4 * a);

	static float Length(Vector4 * a);

	virtual void Print();
};

#endif //__Vector4__
