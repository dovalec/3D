#include "Vector3.h"

NEWDEL_IMPL(Vector3)
Vector3::Vector3() {
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {
	X = x;
	Y = y;
	Z = z;
}

void Vector3::Set(float x, float y, float z) {
	X = x;
	Y = y;
	Z = z;
}

void Vector3::Set(Vector3 * a) {
	X = a->X;
	Y = a->Y;
	Z = a->Z;
}

void Vector3::Add(Vector3 * a) {
	X += a->X;
	Y += a->Y;
	Z += a->Z;
}

void Vector3::Add(float s) {
	X += s;
	Y += s;
	Z += s;
}

void Vector3::Sub(Vector3 * a) {
	X -= a->X;
	Y -= a->Y;
	Z -= a->Z;
}

void Vector3::Mul(Vector3 * a) {
	X *= a->X;
	Y *= a->Y;
	Z *= a->Z;
}

void Vector3::Mul(float s) {
	X *= s;
	Y *= s;
	Z *= s;
}

float Vector3::LengthSquared() {
	return X * X + Y * Y + Z * Z;
}

float Vector3::Length() {
	return sqrt(X * X + Y * Y + Z * Z);
}

void Vector3::Normalize(Vector3 * a) {
	const float length = Length();

	X /= length;
	Y /= length;
	Z /= length;
}

Vector3 * Vector3::Add(Vector3 * a, Vector3 * b, Vector3 * ret) {
	ret->X = a->X + b->X;
	ret->Y = a->Y + b->Y;
	ret->Z = a->Z + b->Z;
	return ret;
}

Vector3 * Vector3::Add(Vector3 * a, float s, Vector3 * ret) {
	ret->X = a->X + s;
	ret->Y = a->Y + s;
	ret->Z = a->Z + s;

	return ret;
}

Vector3 * Vector3::Sub(Vector3 * a, Vector3 * b, Vector3 * ret) {
	ret->X = a->X - b->X;
	ret->Y = a->Y - b->Y;
	ret->Z = a->Z - b->Z;

	return ret;
}

Vector3 * Vector3::Mul(Vector3 * a, Vector3 * b, Vector3 * ret) {
	ret->X = a->X * b->X;
	ret->Y = a->Y * b->Y;
	ret->Z = a->Z * b->Z;

	return ret;
}

Vector3 * Vector3::Mul(Vector3 * a, float s, Vector3 * ret) {
	ret->X = a->X * s;
	ret->Y = a->Y * s;
	ret->Z = a->Z * s;

	return ret;
}

Vector3 * Vector3::CrossProduct(Vector3 * a, Vector3 * b, Vector3 * ret) {
	ret->X = a->Y * b->Z - a->Z * b->Y;
	ret->Y = a->Z * b->X - a->X * b->Z;
	ret->Z = a->X * b->Y - a->Y * b->X;

	return ret;
}

float Vector3::LengthSquared(Vector3 * a) {
	return a->X * a->X + a->Y * a->Y + a->Z * a->Z;
}

float Vector3::Length(Vector3 * a) {
	return sqrt(a->X * a->X + a->Y * a->Y + a->Z * a->Z);
}

Vector3 * Vector3::Normalize(Vector3 * a, Vector3 * ret) {
	const float length = Vector3::Length(a);

	ret->X = a->X / length;
	ret->Y = a->Y / length;
	ret->Z = a->Z / length;

	return ret;
}

void Vector3::Print() {
	//Log::d("Vector3", "X,Y,Z: " + X + "," + Y + "," + Z);
}
