#include "BackCamera.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(BackCamera)
BackCamera::BackCamera()
{
	mCamera = new CameraLookAt();
	mMaxDist = GLOBAL_SCALE*35.0f;
	mHeight = GLOBAL_SCALE*30.0f;

	mFront = 1.0f;
}

BackCamera::~BackCamera()
{
	delete mCamera;
}

void BackCamera::SetNode(Node * pNode)
{
	mNode = pNode;
}

void BackCamera::SetMaxDistance(float dist)
{
	mMaxDist = dist;
}

float BackCamera::GetMaxDistance()
{
	return mMaxDist;
}


void BackCamera::SetLookAtHeight(float height)
{
	mLookAtHeight = height;
}

float BackCamera::GetLookAtHeight()
{
	return mLookAtHeight;
}


void BackCamera::SetHeight(float height)
{
	mHeight = height;
}

float BackCamera::GetHeight()
{
	return mHeight;
}


void BackCamera::OnTick()
{
	
	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();	
	nodeWorldPos.y += mLookAtHeight;

	PVRTVec3 cameraWorldPos = mCamera->GetWorldTranslation();
	PVRTVec3 cameraNewWorldPos(nodeWorldPos.x,mHeight, nodeWorldPos.z + (mFront * mMaxDist));


	mCamera->SetTarget(nodeWorldPos);

	const float avgSize = 5.0f;
	float newX = ((cameraWorldPos.x * (avgSize-1.0f))/avgSize) + (cameraNewWorldPos.x)/avgSize;
	float newZ = ((cameraWorldPos.z * (avgSize-1.0f))/avgSize) + (cameraNewWorldPos.z)/avgSize;

	mCamera->SetTranslation(cameraNewWorldPos);
}


