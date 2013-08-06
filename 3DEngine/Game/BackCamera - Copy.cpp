#include "BackCamera.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(BackCamera)
BackCamera::BackCamera()
{
	mCamera = new Camera();
	mMaxDist = 30.0f;
	mHeight = 10.0f;

	mFront = -1.0f;
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
	//PVRTQUATERNION nodeQuat = mNode->GetRotation();
	PVRTVec3 cameraWorldPos = nodeWorldPos;

	//PVRTQUATERNION nodeQuat;
	//nodeQuat.x = nodeRotation.x;
	//nodeQuat.y = nodeRotation.y;
	//nodeQuat.z = nodeRotation.z;
	//nodeQuat.w = nodeRotation.w;

	mCamera->SetTarget(nodeWorldPos);

	//PVRTVec3 cameraWorldPos = mCamera->GetWorldTranslation();

	const float avgSize = 1;//10.0f;

	//PVRTMATRIX camRotaionMtx;
	//PVRTMatrixRotationQuaternion(camRotaionMtx, nodeQuat);

	//PVRTMat4 mat(camRotaionMtx.f);
	PVRTVec4 zVec(0,mHeight, (mFront * mMaxDist), 1.0f);
	//For rotating camera to player back
	//zVec = mat * zVec;

	zVec.x += nodeWorldPos.x;
	zVec.z += nodeWorldPos.z;
	
	cameraWorldPos.x += zVec.x;
	cameraWorldPos.z += zVec.z;

	//float newX = ((cameraWorldPos.x * (avgSize-1.0f))/avgSize) + (zVec.x)/avgSize;
	//float newZ = ((cameraWorldPos.z * (avgSize-1.0f))/avgSize) + (zVec.z)/avgSize;

	if (mNumStopWires == 0)
	{
		mCamera->SetTranslation(cameraWorldPos.x, mHeight, cameraWorldPos.z);
	}
	else
	{

		float bestDist = 9999999.0f;
		bool colliding = false;
		for (int w = 0 ; w < mNumStopWires; w++)
		{
			Wire * pWire = mStopWires[w];
			for (int i=0;i<pWire->numOfPoints-1 ; i++)
			{
				WirePoint & wA = pWire->wirePoints[i];
				WirePoint & wB = pWire->wirePoints[i+1];
				Line2D line1;
				Line2D line2;

				line1.p0.x = wA.x;
				line1.p0.y = wA.z;
				line1.p1.x = wB.x;
				line1.p1.y = wB.z;

				line2.p0.x = nodeWorldPos.x;
				line2.p0.y = nodeWorldPos.z;

				line2.p1.x = cameraWorldPos.x;
				line2.p1.y = cameraWorldPos.z;

				float interX;
				float interY;
				if (Collision::LineSegmentIntersection(line1, line2, interX, interY))
				{
					PVRTVec2 nodePos(nodeWorldPos.x, nodeWorldPos.z);
					float dist = Collision::Vec2DistanceFromLine2D(line1, nodePos);
					if (dist < bestDist)
					{
						bestDist = dist;
						mCamera->SetTranslation(interX, mHeight, interY);
					}
					colliding = true;					
				}
			}			
		}

		if (!colliding)
		{
			mCamera->SetTranslation(cameraWorldPos.x, mHeight, cameraWorldPos.z);
		}

		//PVRTVec3 cameraBorderPos  = SetCameraPositionAccordingToBroder(mBorderWire, mCameranodeWorldPos.x, mHeight, newZ);
		//mCamera->SetTranslation(cameraBorderPos.x, cameraBorderPos.y, cameraBorderPos.z);
	}
	

	mCamera->Update();
}


