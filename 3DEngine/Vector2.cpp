#include "Vector2.h"

NEWDEL_IMPL(Vector2)
Vector2::Vector2()
{
	X = 0.0f;
	Y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	X = x;
	Y = y;
}

Vector2 * Vector2::Add(Vector2 * a, Vector2 * b, Vector2 * ret)
{
	ret->X = a->X + b->X;
	ret->Y = a->Y + b->Y;
	return ret;
}

Vector2 * Vector2::Add(Vector2 * a, float s, Vector2 * ret)
{
	ret->X = a->X + s;
	ret->Y = a->Y + s;

	return ret;
}

Vector2 * Vector2::Sub(Vector2 * a, Vector2 * b, Vector2 * ret)
{
	ret->X = a->X - b->X;
	ret->Y = a->Y - b->Y;

	return ret;
}

Vector2 * Vector2::Mul(Vector2 * a, Vector2 * b, Vector2 * ret)
{
	ret->X = a->X * b->X;
	ret->Y = a->Y * b->Y;

	return ret;
}

Vector2 * Vector2::Mul(Vector2 * a, float s, Vector2 * ret)
{
	ret->X = a->X * s;
	ret->Y = a->Y * s;

	return ret;
}

float Vector2::LengthSquared(Vector2 * a)
{
	return a->X * a->X + a->Y * a->Y;
}

float Vector2::Length(Vector2 * a)
{
	return sqrt(a->X * a->X + a->Y * a->Y);
}

Vector2 * Vector2::Normalize(Vector2 * a, Vector2 * ret)
{
	const float length = Vector2::Length(a);

	ret->X = a->X / length;
	ret->Y = a->Y / length;

	return ret;
}

void Vector2::Print()
{
	//Log::d("Vector2", "X,Y: " + X + "," + Y);
}
