#ifndef __Vector3__
#define __Vector3__

#include "Config.h"

class Vector3
{
public:
	NEWDEL_DECL

	float X;
	float Y;
	float Z;

	Vector3();

	Vector3(float x, float y, float z);

	virtual void Set(float x, float y, float z);

	virtual void Set(Vector3 * a);

	virtual void Add(Vector3 * a);

	virtual void Add(float s);

	virtual void Sub(Vector3 * a);

	virtual void Mul(Vector3 * a);

	virtual void Mul(float s);

	virtual float LengthSquared();

	virtual float Length();

	virtual void Normalize(Vector3 * a);

	// ////////////////////////////////////////////////////////////////////////////////

	static Vector3 * Add(Vector3 * a, Vector3 * b, Vector3 * ret);

	static Vector3 * Add(Vector3 * a, float s, Vector3 * ret);

	static Vector3 * Sub(Vector3 * a, Vector3 * b, Vector3 * ret);

	static Vector3 * Mul(Vector3 * a, Vector3 * b, Vector3 * ret);

	static Vector3 * Mul(Vector3 * a, float s, Vector3 * ret);

	static Vector3 * CrossProduct(Vector3 * a, Vector3 * b, Vector3 * ret);

	static float LengthSquared(Vector3 * a);

	static float Length(Vector3 * a);

	static Vector3 * Normalize(Vector3 * a, Vector3 * ret);

	virtual void Print();
};

#endif //__Vector3__

