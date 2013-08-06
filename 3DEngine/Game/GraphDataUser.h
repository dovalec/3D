#ifndef __GraphDataUser__
#define __GraphDataUser__

#include "../QuadTree.h"
#include "../GraphData.h"

#define MAX_GRAPH_QUADS 8

class GraphDataUser {

	NEWDEL_DECL;

public:
	GraphDataUser()
	{
		mNumQuadsData = 0;
	}

	void AddQuadTreeData(QuadTree * pQuadTree)
	{
		mQuadsData[mNumQuadsData] = pQuadTree;
		mNumQuadsData++;
		if (mNumQuadsData == MAX_GRAPH_QUADS)
		{
			LOGE("mNumQuadsData == MAX_GRAPH_QUADS");
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

	GraphNode * GetNearestGraphNode(int quadTree, float x, float z);

protected:
	int mNumQuadsData;
	QuadTree * mQuadsData[MAX_GRAPH_QUADS];
	
};


#endif //__GraphDataUser__