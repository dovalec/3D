#include "MoveToTarget.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(MoveToTarget)
MoveToTarget::MoveToTarget()
{
	mWalkDistance = GLOBAL_SCALE*0.1f;
	mMaxDist = GLOBAL_SCALE*24.0f;
	mMinDist = GLOBAL_SCALE*0.5f;
	mNode = NULL;
	mTargetNode = NULL;
	mLookAtTargetNode = NULL;
	mAtTarget = false;
	mFront = 1.0f;
}

MoveToTarget::~MoveToTarget()
{

}

void MoveToTarget::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
	mLookAtTargetNode = mTargetNode; // by default
}

void MoveToTarget::SetTargetPos(PVRTVec3 pos)
{
	mTargetNode = NULL;
	mLookAtTargetNode = NULL;
	mTargetPos = pos;
	mLookAtTargetPos = mTargetPos; // by default
}

void MoveToTarget::SetLookAtTargetNode(Node * pNode)
{
	mLookAtTargetNode = pNode;
}

void MoveToTarget::SetLookAtTargetPos(PVRTVec3 pos)
{
	mLookAtTargetNode = NULL;
	mLookAtTargetPos = pos;
}

void MoveToTarget::SetNode(Node * pNode)
{
	mNode = pNode;
}


void MoveToTarget::SetWalkDistance(float dist)
{
	mWalkDistance = dist;
}

float MoveToTarget::GetWalkDistance()
{
	return mWalkDistance;
}

void MoveToTarget::SetMinDistance(float dist)
{
	mMinDist = dist;
}

float MoveToTarget::GetMinDistance()
{
	return mMinDist;
}


void MoveToTarget::SetMaxDistance(float dist)
{
	mMaxDist = dist;
}

float MoveToTarget::GetMaxDistance()
{
	return mMaxDist;
}

void MoveToTarget::SetFront(float front)
{
	mFront = front;
}

float MoveToTarget::GetFront()
{
	return mFront;
}

void MoveToTarget::CalcNodePos()
{
	if (mTargetNode)
		mTargetPos = mTargetNode->GetWorldTranslation();	

	if (mLookAtTargetNode)
		mLookAtTargetPos = mLookAtTargetNode->GetWorldTranslation();	

	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();	
	PVRTVec3 dir = mTargetPos - nodeWorldPos;
	
	float distLength = dir.lenSqr();
	dir.normalize();

	if (MM(mMinDist) < distLength)
	{
		nodeWorldPos.x += dir.x*mWalkDistance;
		nodeWorldPos.z += dir.z*mWalkDistance;
		mNode->SetTranslation(nodeWorldPos);
		mAtTarget = false;
	}
	else
	{
		mAtTarget = true;
	}


	PVRTVec3 dirLookAt = mLookAtTargetPos - nodeWorldPos;
	float rotAngledirLookAt = atan2f(dirLookAt.z, dirLookAt.x) - PVRT_PI*0.5f;

	PVRTQUATERNION quat;
	PVRTMatrixQuaternionRotationAxis(quat, PVRTVec3(0,1,0),rotAngledirLookAt);

	mNode->SetRotation(quat);
}


