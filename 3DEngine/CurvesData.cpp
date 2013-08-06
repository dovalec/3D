#include "CurvesData.h"

NEWDEL_IMPL(CurvesData)
NEWDEL_IMPL(CurvePoint)
NEWDEL_IMPL(Curve)
CurvesData::CurvesData(void)
{
	
}

CurvesData::~CurvesData(void)
{
	delete [] mCurves;
}

void CurvesData::AllocateCurves(int numOfCurves)
{
	mCurves = new Curve[numOfCurves];
	mNumOfCurves = numOfCurves;
}
