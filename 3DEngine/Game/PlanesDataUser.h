#ifndef __PlanesDataUser__
#define __PlanesDataUser__

#include "../PlanesData.h"
#include "JoyStick.h"

#define MAX_STOP_COL_PLANES 256

class PlanesDataUser {
	NEWDEL_DECL;

public:
	PlanesDataUser()
	{
		mNumPlanesData = 0;
	}

	void AddPlanesData(PlanesData * pPlanesData)
	{
		mPlanesData[mNumPlanesData] = pPlanesData;
		mNumPlanesData++;
		if (mNumPlanesData == MAX_STOP_COL_PLANES)
		{
			LOGE("mNumPlanesData == MAX_STOP_COL_PLANES");
			STOP;
		}
	}

	void RemovePlanesData(PlanesData * pPlanesData)
	{
		for (int n=0;n<mNumPlanesData;n++)
		{
			if (mPlanesData[n] == pPlanesData)
			{
				mNumPlanesData--;
				mPlanesData[n] = mPlanesData[mNumPlanesData];
				break;
			}
		}
	}

	bool CheckCollision(  PVRTVec3 nodeWorldPos, float walkDistance);

protected:
	int mNumPlanesData;
	PlanesData * mPlanesData[MAX_STOP_COL_PLANES];

};


#endif //__PlanesDataUser__