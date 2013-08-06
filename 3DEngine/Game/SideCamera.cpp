#include "BackCamera.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(BackCamera)
BackCamera::BackCamera()
{
	mCamera = new Camera();
	mMaxDist = 12.0f;
	mHeight = 23.0f;
	mBorderWire = NULL;
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


void BackCamera::SetHeight(float height)
{
	mHeight = height;
}

float BackCamera::GetHeight()
{
	return mHeight;
}


void BackCamera::Update()
{
	
	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();

	mCamera->SetTarget(nodeWorldPos);

	PVRTVec3 cameraWorldPos = mCamera->GetWorldTranslation();

	PVRTVec3 distVec = nodeWorldPos - cameraWorldPos;
	float dist = distVec.length();

	const float avgSize = 20.0f;
	float newZ = ((cameraWorldPos.z * (avgSize-1.0f))/avgSize) + (nodeWorldPos.z + mMaxDist)/avgSize;
	
	if (mBorderWire == NULL)
	{
		mCamera->SetTranslation(nodeWorldPos.x, mHeight, newZ);
	}
	else
	{
		PVRTVec2 camPos(cameraWorldPos.x, newZ);

		bool colliding = false;
		for (int i=0;i<mBorderWire->numOfPoints-1 ; i++)
		{
			WirePoint & wA = mBorderWire->wirePoints[i];
			WirePoint & wB = mBorderWire->wirePoints[i+1];
			Line2D line;
			line.p0.x = wA.x;
			line.p0.y = wA.z;

			line.p1.x = wB.x;
			line.p1.y = wB.z;
			
			float dist = Collision::Vec2DistanceFromLine2D(line, camPos);
			if (dist<0.1f)
			{
				PVRTVec2 interPoint = Collision::Vec2CollisionLine2D(line, camPos);
				mCamera->SetTranslation(interPoint.x, mHeight, interPoint.y);
				colliding = true;
				break;
				
			}

		}
		if (!colliding)
		{
			mCamera->SetTranslation(nodeWorldPos.x, mHeight, newZ);
		}

		//PVRTVec3 cameraBorderPos  = SetCameraPositionAccordingToBroder(mBorderWire, mCameranodeWorldPos.x, mHeight, newZ);
		//mCamera->SetTranslation(cameraBorderPos.x, cameraBorderPos.y, cameraBorderPos.z);
	}
	

	mCamera->Update();
}


void BackCamera::SetBorderWire(Wire * pWire)
{
	mBorderWire = pWire;
}