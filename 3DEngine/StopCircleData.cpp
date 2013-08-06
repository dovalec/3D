#include "StopCircleData.h"

NEWDEL_IMPL(StopCircleData)
NEWDEL_IMPL(StopCircle)

StopCircleData::StopCircleData(void)
{
	
}

StopCircleData::~StopCircleData(void)
{
	delete [] mStopCircles;
}


void StopCircleData::AllocateStopCircles(int numofStopCircles)
{
	mStopCircles = new StopCircle[numofStopCircles];
	mNumOfStopCircles = numofStopCircles;
}
