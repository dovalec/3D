#include "GraphDataUser.h"

NEWDEL_IMPL(GraphDataUser);

GraphNode * GraphDataUser::GetNearestGraphNode(int quadTree, float x, float z)
{
	if (quadTree >= mNumQuadsData)
	{
		STOP;
	}

	static QuadNode * quadNodes[256]={0}; 
	int quadNodeCount = 0;


	PVRTVec2 requestedPos(x,z);

	GraphNode * bestGraphNode = NULL;
	mQuadsData[quadTree]->GetQuads(x, z, 0,quadNodes, quadNodeCount);
	quadNodeCount--;


	float bestDist = 9999999999.0f;

	for (int quad = quadNodeCount ; quad >=0 ; quad--)
	{
		QuadNode * pQuadNode = quadNodes[quad];

		List & dataList = pQuadNode->GetDataList();
		ListIterator listIter(dataList);
		while( GraphNode * pGraphNode = (GraphNode*)listIter.GetPtr())	
		{			
			PVRTVec2 nodePos(pGraphNode->x, pGraphNode->z);
			PVRTVec2 distVec = nodePos - requestedPos;
			float dist = distVec.length();

			if ( dist < bestDist ) 
			{
				bestDist = dist;
				bestGraphNode = pGraphNode;
			}		
		}		
	}

	return bestGraphNode;
}