#include "BezierCurve.h"



void BezierCurve::PutValues(array<Vector3^> * points)
{
	mPoints = points;
}

Vector3 * BezierCurve::getPoint(float time, Vector3 * point)
{

	float muk = 1;
	float munk = safe_cast<float>(Math::Pow(1 - time, mPoints->length - 1));

	point->X = 0;
	point->Y = 0;
	point->Z = 0;

	for (int i = 0; i < mPoints->length; i++)
	{
		int count = mPoints->length - 1;
		int iCount = i;
		int diff = count - iCount;
		float blend = muk * munk;
		muk *= time;
		munk /= (1 - time);
		while (count >= 1)
		{
			blend *= count;
			count--;
			if (iCount > 1)
			{
				blend /= iCount;
				iCount--;
			}

			if (diff > 1)
			{
				blend /= diff;
				diff--;
			}
		}

		mTempVec = mPoints[i];
		point->X += mTempVec->X * blend;
		point->Y += mTempVec->Y * blend;
		point->Z += mTempVec->Z * blend;
	}

	return point;
}

Vector3 * BezierCurve::getPoint(float time)
{
	return getPoint(time, new Vector3());
}

Matrix * BezierCurve::getOrientation(float time, float precision)
{
	Matrix * rotation = new Matrix();

	// calculate tangent
	Vector3 * point = getPoint(time);
	Vector3 * tangent = new Vector3(); // point.subtract(getPoint(time +
	// precision));
	Vector3::Sub(point, getPoint(time + precision), tangent);
	Vector3::Normalize(tangent, tangent); // tangent.normalize();
	// calculate normal
	Vector3 * tangent2 = new Vector3(); // = getPoint(time -
	// precision).subtract(point);
	Vector3::Sub(getPoint(time - precision), point, tangent2);
	Vector3 * normal = new Vector3(); // = tangent.cross(tangent2);
	Vector3::CrossProduct(tangent, tangent2, normal);
	// normal = normal.normalize();
	Vector3::Normalize(normal, normal);
	// calculate binormal
	Vector3 * binormal = new Vector3(); // = tangent.cross(normal);
	Vector3::CrossProduct(tangent, normal, binormal);
	// binormal = binormal.normalize();
	Vector3::Normalize(binormal, binormal);

	rotation->M[0] = tangent->X;
	rotation->M[4] = tangent->Y;
	rotation->M[8] = tangent->Z;

	rotation->M[1] = normal->X;
	rotation->M[5] = normal->Y;
	rotation->M[9] = normal->Z;

	rotation->M[2] = binormal->X;
	rotation->M[6] = binormal->Y;
	rotation->M[10] = binormal->Z;

	//        
	//		 * rotation.setColumn(0, tangent); rotation.setColumn(1, normal);
	//		 * rotation.setColumn(2, binormal);
	//		 

	return rotation;
}

Matrix * BezierCurve::getOrientation(float time, float precision, Vector3 * up)
{
	if (up == nullptr)
	{
		return getOrientation(time, precision);
	}
	Matrix * rotation = new Matrix();

	// calculate tangent
	Vector3 * tangent = new Vector3(); // getPoint(time).subtract(getPoint(time
	// + precision));
	Vector3::Sub(getPoint(time), getPoint(time + precision), tangent);
	// tangent = tangent.normalize();
	Vector3::Normalize(tangent, tangent);

	// calculate binormal
	Vector3 * binormal = new Vector3(); // tangent.cross(up);
	Vector3::CrossProduct(tangent, up, binormal);
	// binormal = binormal.normalize();
	Vector3::Normalize(binormal, binormal);

	// calculate normal
	Vector3 * normal = new Vector3();
	// binormal.cross(tangent);
	Vector3::CrossProduct(binormal, tangent, normal);

	// normal = normal.normalize();
	Vector3::Normalize(normal, normal);

	rotation->M[0] = tangent->X;
	rotation->M[4] = tangent->Y;
	rotation->M[8] = tangent->Z;

	rotation->M[1] = normal->X;
	rotation->M[5] = normal->Y;
	rotation->M[9] = normal->Z;

	rotation->M[2] = binormal->X;
	rotation->M[6] = binormal->Y;
	rotation->M[10] = binormal->Z;

	//        
	//		 * rotation.setColumn(0, tangent); rotation.setColumn(1, normal);
	//		 * rotation.setColumn(2, binormal);
	//		 
	return rotation;
}
