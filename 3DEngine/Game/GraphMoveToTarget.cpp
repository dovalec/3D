#include "GraphMoveToTarget.h"

#include "../ResourceLoader.h"
#include "../DataTable.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(GraphMoveToTarget)
GraphMoveToTarget::GraphMoveToTarget()
{
	mWalkDistance = GLOBAL_SCALE*0.1f;
	mMaxDist = GLOBAL_SCALE*24.0f;
	mMinDist = GLOBAL_SCALE*0.5f;
	mNode = NULL;
	mTargetNode = NULL;
	mAtTarget = false;
	mFront = 1.0f;

	mRefSceneData = NULL;
	mTargetGraphNode = NULL;
	mCurrentGraphNode = NULL;
	mNextGraphNode = NULL;

	mRefreshRate = 5;

	mTickCount = 0;
	mLastDist = 9999999.0f;
	mLastGraphNode = NULL;

	mMoveMode = GraphMoveToTarget::TRANSLATE_ROTATE;
}

GraphMoveToTarget::~GraphMoveToTarget()
{

}

void GraphMoveToTarget::InitQuads()
{
	if (mRefSceneData == NULL)
	{
		DataTable & dataTable = DataTable::GetDataTable();
		mRefSceneData = (RefSceneData*)dataTable.GetDataByName("LevelRefSceneData");
		for (int n=0;n<mRefSceneData->mNumOfGraphQuadTree;n++)
		{
			AddQuadTreeData( &mRefSceneData->mGraphQuadTree[n]);
		}
	}
}

void GraphMoveToTarget::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;



}

void GraphMoveToTarget::SetTargetPos(PVRTVec3 pos)
{
	mTargetNode = NULL;
	mTargetPos = pos;
}

void GraphMoveToTarget::SetNode(Node * pNode)
{
	mNode = pNode;
}


void GraphMoveToTarget::SetWalkDistance(float dist)
{
	mWalkDistance = dist;
}

float GraphMoveToTarget::GetWalkDistance()
{
	return mWalkDistance;
}

void GraphMoveToTarget::SetMinDistance(float dist)
{
	mMinDist = dist;
}

float GraphMoveToTarget::GetMinDistance()
{
	return mMinDist;
}


void GraphMoveToTarget::SetMaxDistance(float dist)
{
	mMaxDist = dist;
}

float GraphMoveToTarget::GetMaxDistance()
{
	return mMaxDist;
}

void GraphMoveToTarget::SetFront(float front)
{
	mFront = front;
}

float GraphMoveToTarget::GetFront()
{
	return mFront;
}

GraphNode * GraphMoveToTarget::FindRandomGraphNode()
{
	float randPosX = GLOBAL_SCALE*(float)(rand() % 1000 - 500);
	float randPosY = GLOBAL_SCALE*(float)(rand() % 1000 - 500);

	GraphNode * pGraphNode = GetNearestGraphNode( 0, randPosX, randPosY );
	if (pGraphNode == NULL)
	{
		STOP;
	}

	return pGraphNode;
}

void GraphMoveToTarget::SetCurrentGraphNodeRandom()
{
	mCurrentGraphNode = FindRandomGraphNode();
	SearchNextGraphNode();
}

void GraphMoveToTarget::SearchNextGraphNode()
{
	
	GraphNode ** pEdges = mCurrentGraphNode->GetEdges();	

	PVRTVec2 currentPos( mCurrentGraphNode->x, mCurrentGraphNode->z);
	PVRTVec2 targetPos( mTargetPos.x, mTargetPos.z);

	PVRTVec2 dirTarget = targetPos - currentPos;
	dirTarget.normalize();

	float bestAngle = 999999.0f;

	bool not_found = true;

	int numEdges = mCurrentGraphNode->GetNumEdges();
	for (int edge = 0; edge < numEdges ; edge++)
	{
		GraphNode * pEdgeNode = pEdges[edge];
		if (pEdgeNode->IsTaken())
			continue;

		PVRTVec2 edgePos( pEdgeNode->x, pEdgeNode->z);
		PVRTVec2 dirEdge = edgePos - currentPos;
		dirEdge.normalize();

		float deltaAngle = acos ( dirTarget.dot(dirEdge) );
		if ( bestAngle > deltaAngle && pEdgeNode != mLastGraphNode )
		{
			bestAngle = deltaAngle;			
			mNextGraphNode = pEdgeNode;
			not_found = false;
		}
	}

	if (not_found)
	{
		mNextGraphNode = pEdges[0];
	}

	mNextGraphNode->SetTaken(true);

	SetupDirection();
}

void GraphMoveToTarget::SetupDirection()
{
	mNextPos.x = mNextGraphNode->x;
	mNextPos.y = mNextGraphNode->y;
	mNextPos.z = mNextGraphNode->z;

	static int randIndex = 0;
	const static float randTable[20] = {1,4,3,2,5,3,6,5,2,3,4,2,4,5,5,3,4,5,6,3};

	float r = mWalkDistance * (float)randTable[(randIndex++%20)];

	mCurrentPos.x = mCurrentGraphNode->x + r;
	mCurrentPos.y = mCurrentGraphNode->y;
	mCurrentPos.z = mCurrentGraphNode->z + r;

	mDirection = mNextPos - mCurrentPos;
	mDirection.normalize();

	float rotAngleDirLookAt = atan2f(mDirection.z, mDirection.x) - PVRT_PI*0.5f;

	PVRTMatrixQuaternionRotationAxis(mRotQuat, PVRTVec3(0,1,0),rotAngleDirLookAt);

}

void GraphMoveToTarget::OnTick()
{
	if (mTargetNode)
		mTargetPos = mTargetNode->GetWorldTranslation();	

	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();	
	
/*
	if ( !(mTickCount%mRefreshRate) )
	{
		mTargetGraphNode = GetNearestGraphNode( 0, mTargetPos.x, mTargetPos.z );
		if (mTargetGraphNode == NULL)
		{
			STOP;
		}

	}
*/

	PVRTVec3 posToNext = mNextPos - nodeWorldPos;
	float distToNext = posToNext.lenSqr();
	
	//if (MM(mMinDist*2.0f) > distLength )
	if (distToNext > mLastDist)
	{
		/*if (mTargetGraphNode == mNextGraphNode)
		{
			mAtTarget = true;
		}
		else
		*/
		{			
			mAtTarget = false;
			mLastGraphNode = mCurrentGraphNode;
			mLastGraphNode->SetTaken( false );
			mCurrentGraphNode = mNextGraphNode;			
			SearchNextGraphNode();
			mLastDist = 9999999.0f;
		}
	}	
	else
	{
		mLastDist = distToNext;
	}
	
    Shell * pShell = Shell::GetShell();
    
    float fpsFactor = pShell->GetFPSFactor();
	nodeWorldPos.x += mDirection.x*mWalkDistance*fpsFactor;
	nodeWorldPos.z += mDirection.z*mWalkDistance*fpsFactor;

	if ( mMoveMode == GraphMoveToTarget::TRANSLATE_ROTATE || mMoveMode == GraphMoveToTarget::TRANSLATE )
		mNode->SetTranslation(nodeWorldPos);	

	if ( mMoveMode == GraphMoveToTarget::TRANSLATE_ROTATE || mMoveMode == GraphMoveToTarget::ROTATE )
		mNode->SetRotation(mRotQuat);

	mTickCount++;
}


