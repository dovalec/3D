#ifndef __StopCircleUser__
#define __StopCircleUser__

#include "../StopCircleData.h"
#include "JoyStick.h"

#define MAX_STOP_CIRCLES 256

class StopCircleUser {
public:
	NEWDEL_DECL;

	StopCircleUser()
	{
		mNumStopCircles = 0;
	}

	void AddStopCircle(StopCircle * pStopCircle)
	{
		mStopCircles[mNumStopCircles] = pStopCircle;
		mNumStopCircles++;
		if (mNumStopCircles == MAX_STOP_CIRCLES)
		{
			LOGE("mNumStopCircles == MAX_STOP_CIRCLES");
			STOP;
		}
	}

	void RemoveStopCircle(StopCircle * pStopCircle)
	{
		for (int n=0;n<mNumStopCircles;n++)
		{
			if (mStopCircles[n] == pStopCircle)
			{
				mNumStopCircles--;
				mStopCircles[n] = mStopCircles[mNumStopCircles];
				break;
			}
		}
	}

	bool CheckCollision( PVRTVec3 nodeWorldPos);

protected:
	int mNumStopCircles;
	StopCircle * mStopCircles[MAX_STOP_CIRCLES];

};


#endif //__StopCircleUser__