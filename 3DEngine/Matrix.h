#ifndef __Matrix__
#define __Matrix__


#include "Config.h"

#include "Vector3.h"
#include "Vector4.h"

class Matrix {
public:
	NEWDEL_DECL
	float M[16];
	float mTranspose[16];

	Matrix();

	Matrix(float * floats);
	Matrix(Matrix * src);

	void SetValues(float * floats);

	static Matrix * Copy(Matrix * dst, Matrix * src);

	static Matrix * Identity(Matrix * ret);

	virtual void Identity();

	static Matrix * Multiply(Matrix * a, Matrix * b, Matrix * ret);

	static Vector3 * Transform(Vector3 * vec, Matrix * mtx, Vector3 * ret);

	static Vector4 * Transform(Vector4 * vec, Matrix * mtx, Vector4 * ret);

	static Matrix * CreateTranslation(Vector3 * vec, Matrix * mtx);

	static Matrix * CreateTranslationXYZ(float X, float Y, float Z,
			Matrix * mtx);

	static Matrix * CreateTranslationXYZFast(float X, float Y, float Z,
			Matrix * mtx);

	static Matrix * CreateTranslationX(float X, Matrix * mtx);

	static Matrix * CreateTranslationXFast(float X, Matrix * mtx);

	static Matrix * CreateTranslationY(float Y, Matrix * mtx);

	static Matrix * CreateTranslationYFast(float Y, Matrix * mtx);

	static Matrix * CreateTranslationZ(float Z, Matrix * mtx);

	static Matrix * CreateTranslationZFast(float Z, Matrix * mtx);

	static Matrix * CreateScale(Vector3 * vec, Matrix * mtx);

	static Matrix * CreateScaleXYZ(float X, float Y, float Z, Matrix * mtx);

	static Matrix * CreateScaleX(float X, Matrix * mtx);

	static Matrix * CreateScaleY(float Y, Matrix * mtx);

	static Matrix * CreateScaleZ(float Z, Matrix * mtx);

	static Matrix * CreateRotateX(float X, Matrix * mtx);

	static Matrix * CreateRotateY(float Y, Matrix * mtx);

	static Matrix * CreateRotateZ(float Z, Matrix * mtx);

	virtual void Copy(Matrix * src);

	virtual void Copy(float * values);
	
	virtual float * Transpose();

	virtual void Print();
};

#endif //__Matrix__
