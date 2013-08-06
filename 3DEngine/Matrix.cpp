#include "Matrix.h"

NEWDEL_IMPL(Matrix)
Matrix::Matrix()
{
	Matrix::Identity(this);
}

Matrix::Matrix(float * floats)
{
	this->Copy(floats);
}

Matrix::Matrix(Matrix * src)
{
	Matrix::Copy(this, src);
}

Matrix * Matrix::Copy(Matrix * dst, Matrix * src)
{
	*dst->M = *src->M;	
	return dst;
}

void Matrix::SetValues(float * floats)
{
	this->Copy(floats);
}

Matrix * Matrix::Identity(Matrix * ret)
{
	ret->M[0] = 1.0f;
	ret->M[1] = 0.0f;
	ret->M[2] = 0.0f;
	ret->M[3] = 0.0f;
	ret->M[4] = 0.0f;
	ret->M[5] = 1.0f;
	ret->M[6] = 0.0f;
	ret->M[7] = 0.0f;
	ret->M[8] = 0.0f;
	ret->M[9] = 0.0f;
	ret->M[10] = 1.0f;
	ret->M[11] = 0.0f;
	ret->M[12] = 0.0f;
	ret->M[13] = 0.0f;
	ret->M[14] = 0.0f;
	ret->M[15] = 1.0f;

	return ret;
}

void Matrix::Identity()
{
	M[0] = 1.0f;
	M[1] = 0.0f;
	M[2] = 0.0f;
	M[3] = 0.0f;
	M[4] = 0.0f;
	M[5] = 1.0f;
	M[6] = 0.0f;
	M[7] = 0.0f;
	M[8] = 0.0f;
	M[9] = 0.0f;
	M[10] = 1.0f;
	M[11] = 0.0f;
	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}


Matrix * Matrix::Multiply(Matrix * a, Matrix * b, Matrix * ret)
{
	//
	//		 for ( int i=0; i<4; i++)
	//		 {
	//	            for ( int j=0; j<4; j++)
	//	            {
	//	            	ret.M[i*4+j] = (a.M[i*4+0]*b.M[0+4*j] +
	//	            					a.M[i*4+1]*b.M[1+4*j] +
	//	            					a.M[i*4+2]*b.M[2+4*j] +
	//	            					a.M[i*4+3]*b.M[3+4*j]);
	//	            }
	//		 }
	//		 
	//		
	//		

	ret->M[0] = a->M[0] * b->M[0] + a->M[1] * b->M[4] + a->M[2] * b->M[8] + a->M[3] * b->M[12];
	ret->M[1] = a->M[0] * b->M[1] + a->M[1] * b->M[5] + a->M[2] * b->M[9] + a->M[3] * b->M[13];
	ret->M[2] = a->M[0] * b->M[2] + a->M[1] * b->M[6] + a->M[2] * b->M[10] + a->M[3] * b->M[14];
	ret->M[3] = a->M[0] * b->M[3] + a->M[1] * b->M[7] + a->M[2] * b->M[11] + a->M[3] * b->M[15];

	ret->M[4] = a->M[4] * b->M[0] + a->M[5] * b->M[4] + a->M[6] * b->M[8] + a->M[7] * b->M[12];
	ret->M[5] = a->M[4] * b->M[1] + a->M[5] * b->M[5] + a->M[6] * b->M[9] + a->M[7] * b->M[13];
	ret->M[6] = a->M[4] * b->M[2] + a->M[5] * b->M[6] + a->M[6] * b->M[10] + a->M[7] * b->M[14];
	ret->M[7] = a->M[4] * b->M[3] + a->M[5] * b->M[7] + a->M[6] * b->M[11] + a->M[7] * b->M[15];

	ret->M[8] = a->M[8] * b->M[0] + a->M[9] * b->M[4] + a->M[10] * b->M[8] + a->M[11] * b->M[12];
	ret->M[9] = a->M[8] * b->M[1] + a->M[9] * b->M[5] + a->M[10] * b->M[9] + a->M[11] * b->M[13];
	ret->M[10] = a->M[8] * b->M[2] + a->M[9] * b->M[6] + a->M[10] * b->M[10] + a->M[11] * b->M[14];
	ret->M[11] = a->M[8] * b->M[3] + a->M[9] * b->M[7] + a->M[10] * b->M[11] + a->M[11] * b->M[15];

	ret->M[12] = a->M[12] * b->M[0] + a->M[13] * b->M[4] + a->M[14] * b->M[8] + a->M[15] * b->M[12];
	ret->M[13] = a->M[12] * b->M[1] + a->M[13] * b->M[5] + a->M[14] * b->M[9] + a->M[15] * b->M[13];
	ret->M[14] = a->M[12] * b->M[2] + a->M[13] * b->M[6] + a->M[14] * b->M[10] + a->M[15] * b->M[14];
	ret->M[15] = a->M[12] * b->M[3] + a->M[13] * b->M[7] + a->M[14] * b->M[11] + a->M[15] * b->M[15];

	return ret;
}

Vector3 * Matrix::Transform(Vector3 * vec, Matrix * mtx, Vector3 * ret)
{
	ret->X = mtx->M[0] * vec->X + mtx->M[1] * vec->Y + mtx->M[2] * vec->Z;
	ret->Y = mtx->M[4] * vec->X + mtx->M[5] * vec->Y + mtx->M[6] * vec->Z;
	ret->Z = mtx->M[8] * vec->X + mtx->M[9] * vec->Y + mtx->M[10] * vec->Z;
	return ret;
}

Vector4 * Matrix::Transform(Vector4 * vec, Matrix * mtx, Vector4 * ret)
{
	ret->X = mtx->M[0] * vec->X + mtx->M[1] * vec->Y + mtx->M[2] * vec->Z + mtx->M[3] * vec->W;
	ret->Y = mtx->M[4] * vec->X + mtx->M[5] * vec->Y + mtx->M[6] * vec->Z + mtx->M[7] * vec->W;
	ret->Z = mtx->M[8] * vec->X + mtx->M[9] * vec->Y + mtx->M[10] * vec->Z + mtx->M[11] * vec->W;
	ret->W = mtx->M[12] * vec->X + mtx->M[13] * vec->Y + mtx->M[14] * vec->Z + mtx->M[15] * vec->W;

	return ret;
}

Matrix * Matrix::CreateTranslation(Vector3 * vec, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[3] = vec->X;
	mtx->M[7] = vec->Y;
	mtx->M[11] = vec->Z;
	return mtx;
}

Matrix * Matrix::CreateTranslationXYZ(float X, float Y, float Z, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[3] = X;
	mtx->M[7] = Y;
	mtx->M[11] = Z;
	return mtx;
}

Matrix * Matrix::CreateTranslationXYZFast(float X, float Y, float Z, Matrix * mtx)
{
	mtx->M[3] = X;
	mtx->M[7] = Y;
	mtx->M[11] = Z;
	return mtx;
}

Matrix * Matrix::CreateTranslationX(float X, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[3] = X;
	return mtx;
}

Matrix * Matrix::CreateTranslationXFast(float X, Matrix * mtx)
{
	mtx->M[3] = X;
	return mtx;
}

Matrix * Matrix::CreateTranslationY(float Y, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[7] = Y;
	return mtx;
}

Matrix * Matrix::CreateTranslationYFast(float Y, Matrix * mtx)
{
	mtx->M[7] = Y;
	return mtx;
}

Matrix * Matrix::CreateTranslationZ(float Z, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[11] = Z;
	return mtx;
}

Matrix * Matrix::CreateTranslationZFast(float Z, Matrix * mtx)
{
	mtx->M[11] = Z;
	return mtx;
}

Matrix * Matrix::CreateScale(Vector3 * vec, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[0] = vec->X;
	mtx->M[5] = vec->Y;
	mtx->M[10] = vec->Z;
	return mtx;
}

Matrix * Matrix::CreateScaleXYZ(float X, float Y, float Z, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[0] = X;
	mtx->M[5] = Y;
	mtx->M[10] = Z;
	return mtx;
}

Matrix * Matrix::CreateScaleX(float X, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[0] = X;
	return mtx;
}

Matrix * Matrix::CreateScaleY(float Y, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[5] = Y;
	return mtx;
}

Matrix * Matrix::CreateScaleZ(float Z, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[10] = Z;
	return mtx;
}

Matrix * Matrix::CreateRotateX(float X, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[5] = cos(X);
	mtx->M[6] = -sin(X);
	mtx->M[9] = sin(X);
	mtx->M[10] = cos(X);
	return mtx;
}

Matrix * Matrix::CreateRotateY(float Y, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[0] = cos(Y);
	mtx->M[2] = sin(Y);
	mtx->M[8] = -sin(Y);
	mtx->M[10] = cos(Y);
	return mtx;
}

Matrix * Matrix::CreateRotateZ(float Z, Matrix * mtx)
{
	Matrix::Identity(mtx);
	mtx->M[0] = cos(Z);
	mtx->M[1] = -sin(Z);
	mtx->M[4] = sin(Z);
	mtx->M[5] = cos(Z);
	return mtx;
}

void Matrix::Copy(Matrix * src)
{
	memcpy(M, src->M, sizeof(float)*16);
}

void Matrix::Copy(float * values)
{
	memcpy(M, values, sizeof(float)*16);
}


float * Matrix::Transpose()
{
	mTranspose[0] = M[0];
	mTranspose[5] = M[5];
	mTranspose[10] = M[10];
	mTranspose[15] = M[15];

	mTranspose[4] = M[1];
	mTranspose[8] = M[2];
	mTranspose[12] = M[3];
	mTranspose[9] = M[6];
	mTranspose[13] = M[7];
	mTranspose[14] = M[11];
	return mTranspose;
}

void Matrix::Print()
{
	/*
	Log::d("Matrix","");
	Log::d("Matrix", M[0] + "," + M[1] + "," + M[2] + "," + M[3]);
	Log::d("Matrix", M[4] + "," + M[5] + "," + M[6] + "," + M[7]);
	Log::d("Matrix", M[8] + "," + M[9] + "," + M[10] + "," + M[11]);
	Log::d("Matrix", M[12] + "," + M[13] + "," + M[14] + "," + M[15]);
	*/
}

