#ifndef __CollisionDataUser__
#define __CollisionDataUser__

#include "../QuadTree.h"
#include "JoyStick.h"

#define MAX_QUADS 256

class CollisionDataUser {
	NEWDEL_DECL;

public:
	CollisionDataUser()
	{
		mNumQuadsData = 0;
		mLastPlayerPos*=0.0f;
		mCollided = false;
	}

	void AddQuadTreeData(QuadTree * pQuadTree)
	{
		mQuadsData[mNumQuadsData] = pQuadTree;
		mNumQuadsData++;
		if (mNumQuadsData == MAX_QUADS)
		{
			LOGE("mNumQuadsData == MAX_QUADS");
			STOP;
		}
	}

	void RemoveQuadTreeData(QuadTree * pQuadTree)
	{
		for (int n=0;n<mNumQuadsData;n++)
		{
			if (mQuadsData[n] == pQuadTree)
			{
				mNumQuadsData--;
				mQuadsData[n] = mQuadsData[mNumQuadsData];
				break;
			}
		}
	}

	bool CheckCollision( Node * pNode , float walkDistance, bool stopOnFirstCollision);

protected:
	int mNumQuadsData;
	QuadTree * mQuadsData[MAX_QUADS];
	PVRTVec2 mLastPlayerPos; 
	bool mCollided;

};


#endif //__CollisionDataUser__