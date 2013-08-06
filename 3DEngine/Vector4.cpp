#include "Vector4.h"

NEWDEL_IMPL(Vector4)
Vector4::Vector4()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
	W = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Vector4 * Vector4::Add(Vector4 * a, Vector4 * b, Vector4 * ret)
{
	ret->X = a->X + b->X;
	ret->Y = a->Y + b->Y;
	ret->Z = a->Z + b->Z;
	ret->W = a->W + b->W;

	return ret;
}

Vector4 * Vector4::Add(Vector4 * a, float s, Vector4 * ret)
{
	ret->X = a->X + s;
	ret->Y = a->Y + s;
	ret->Z = a->Z + s;
	ret->W = a->W + s;

	return ret;
}

Vector4 * Vector4::Sub(Vector4 * a, Vector4 * b, Vector4 * ret)
{
	ret->X = a->X - b->X;
	ret->Y = a->Y - b->Y;
	ret->Z = a->Z - b->Z;
	ret->W = a->W - b->W;

	return ret;
}

Vector4 * Vector4::Mul(Vector4 * a, Vector4 * b, Vector4 * ret)
{
	ret->X = a->X * b->X;
	ret->Y = a->Y * b->Y;
	ret->Z = a->Z * b->Z;
	ret->W = a->W * b->W;

	return ret;
}

Vector4 * Vector4::Mul(Vector4 * a, float s, Vector4 * ret)
{
	ret->X = a->X * s;
	ret->Y = a->Y * s;
	ret->Z = a->Z * s;
	ret->W = a->W * s;

	return ret;
}

Vector4 * Vector4::CrossProduct(Vector4 * a, Vector4 * b, Vector4 * ret)
{
	ret->X = a->Y * b->Z - a->Z * b->Y;
	ret->Y = a->Z * b->W - a->W * b->Z;
	ret->Z = a->W * b->X - a->X * b->W;
	ret->W = a->X * b->Y - a->Y * b->X;

	return ret;
}

float Vector4::LengthSquared(Vector4 * a)
{
	return a->X * a->X + a->Y * a->Y + a->Z * a->Z;
}

float Vector4::Length(Vector4 * a)
{
	return sqrt(a->X * a->X + a->Y * a->Y + a->Z * a->Z);
}

void Vector4::Print()
{
	//Log::d("Vector4", "X,Y,Z,W: " + X + "," + Y + "," + Z + "," + W);
}
