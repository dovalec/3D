#ifndef __StopCircleData__
#define __StopCircleData__

#include "Config.h"

struct StopCircle
{
	NEWDEL_DECL
	float x;
	float y;
	float z;

	float radius;
};

class StopCircleData
{
public:
	NEWDEL_DECL
	StopCircleData(void);
	~StopCircleData(void);

	void AllocateStopCircles(int numofStopCircles);

	inline int GetNumOfStopCircles()
	{
		return mNumOfStopCircles;
	}

	inline StopCircle * GetStopCircle(int index)
	{
		return &mStopCircles[index];
	}

	StopCircle * mStopCircles;
	int mNumOfStopCircles;
};


#endif //__StopCircleData__