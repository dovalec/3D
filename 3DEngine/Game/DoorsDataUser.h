#ifndef __DoorsDataUser__
#define __DoorsDataUser__

#include "../DoorsData.h"
#include "JoyStick.h"

#define MAX_STOP_DOORS_DATA 32

class DoorsDataUser {
public:
	NEWDEL_DECL;

	DoorsDataUser()
	{
		mNumDoorsData = 0;
	}

	void AddDoorsData(DoorsData * pDoorsData)
	{
		mDoorsData[mNumDoorsData] = pDoorsData;
		mNumDoorsData++;
		if (mNumDoorsData == MAX_STOP_DOORS_DATA)
		{
			LOGE("mNumDoorsData == MAX_STOP_DOORS_DATA");
			STOP;
		}
	}

	void RemoveDoorsData(DoorsData * pDoorsData)
	{
		for (int n=0;n<mNumDoorsData;n++)
		{
			if (mDoorsData[n] == pDoorsData)
			{
				mNumDoorsData--;
				mDoorsData[n] = mDoorsData[mNumDoorsData];
				break;
			}
		}
	}

	bool CheckCollision( PVRTVec3 nodeWorldPos, float walkDistance);

protected:
	int mNumDoorsData;
	DoorsData * mDoorsData[MAX_STOP_DOORS_DATA];

};


#endif //__DoorsDataUser__