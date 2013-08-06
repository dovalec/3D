#ifndef __CurvesData__
#define __CurvesData__

#include "Config.h"
#include "TL/Map.h"
#include "TL/MyString.h"

struct CurvePoint {
	NEWDEL_DECL
	float x;
	float y;
	float z;

	float tx;
	float ty;
	float tz;
};

struct Curve
{
public:
	NEWDEL_DECL
	Curve()
	{
	}
	virtual ~Curve()
	{
		delete [] curvePoints;
	}
	int numOfPoints;
	String name;
	CurvePoint * curvePoints;
};

class CurvesData
{
public:
	NEWDEL_DECL
	CurvesData(void);
	virtual ~CurvesData(void);

	inline int GetNumOfCurves()
	{
		return mNumOfCurves;
	}

	inline Curve * GetCurve(int index)
	{
		return &mCurves[index];
	}

	inline Curve * GetCurve(String name)
	{
		int index = (int)mCurvesTable.GetByKey(name.GetPtr());
		return &mCurves[index];
	}

	void AllocateCurves(int numOfCurves);

public:
	Map  mCurvesTable;

private:
	int mNumOfCurves;
	Curve * mCurves;
};


#endif //__CurvesData__