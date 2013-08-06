#ifndef __BezierCurve__
#define __BezierCurve__

#include "Config.h"

#include "Vector3.h"
#include "Matrix.h"


class BezierCurve
{

public:
	array<Vector3*> * mPoints;
private:
	static Vector3 * mTempVec = new Vector3();

public:
	virtual void PutValues(array<Vector3*> * points);

	virtual Vector3 * getPoint(float time, Vector3 * point);

	virtual Vector3 * getPoint(float time);

	//    *
	//	 * <code>getOrientation</code> calculates the rotation matrix for any given
	//	 * point along to the line to still be facing in the direction of the line.
	//	 * 
	//	 * @param time
	//	 *            the current time (between 0 and 1)
	//	 * @param precision
	//	 *            how accurate to (i.e. the next time) to check against.
	//	 * @return the rotation matrix.
	//	 * @see com.jme.curve.Curve#getOrientation(float, float)
	//	 
	virtual Matrix * getOrientation(float time, float precision);

	//    *
	//	 * <code>getOrientation</code> calculates the rotation matrix for any given
	//	 * point along to the line to still be facing in the direction of the line.
	//	 * A up vector is supplied, this keep the rotation matrix following the
	//	 * line, but insures the object's up vector is not drastically changed.
	//	 * 
	//	 * @param time
	//	 *            the current time (between 0 and 1)
	//	 * @param precision
	//	 *            how accurate to (i.e. the next time) to check against.
	//	 * @return the rotation matrix.
	//	 * @see com.jme.curve.Curve#getOrientation(float, float)
	//	 
	virtual Matrix * getOrientation(float time, float precision, Vector3 * up);

};

#endif //__BezierCurve__

