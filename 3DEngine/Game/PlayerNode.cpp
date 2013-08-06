#include "PlayerNode.h"
#include "../Shell.h"
#include "../CameraManager.h"

NEWDEL_IMPL(PlayerNode)
PlayerNode::PlayerNode()
{
	
	mWalkDistance = GLOBAL_SCALE*0.0f;
	mNumTriggerWires = 0;
	mModelData = NULL;
	mNode = NULL;
	mStopOnFirstCollision = false;
	mDontWalk = false;
    mDontRotate = false;

	mRotMtxLeft.Identity();
	mRotMtxRight.Identity();

	mLeftJoy*=0.0f;
	mRightJoy*=0.0f;

	mLastRightJoystickPressed = false;
    
    mSmoothRotate = false;
    
}

PlayerNode::~PlayerNode()
{

}

void PlayerNode::SetStopOnFirstCollision(bool state)
{
	mStopOnFirstCollision = state;
}

void PlayerNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
	mNode = mModelData->GetRoot();
}


void PlayerNode::SetWalkDistance(float dist)
{
	mWalkDistance = dist;
}

float PlayerNode::GetWalkDistance()
{
	return mWalkDistance;
}

bool PlayerNode::CheckCollision()
{

	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();

	bool doorsPlanes = DoorsDataUser::CheckCollision(nodeWorldPos, mWalkDistance);
	if (doorsPlanes)
		return doorsPlanes;


	bool collisionPlanes = CollisionDataUser::CheckCollision( mNode, mWalkDistance, mStopOnFirstCollision);
	if (collisionPlanes)
		return collisionPlanes;

	bool collisionCircles = StopCircleUser::CheckCollision( nodeWorldPos);
	if (collisionCircles)
		return collisionCircles;

	return false;

	/*

	/// StopWire:
	if (!colliding) // if not colliding circles
	{
	PVRTVec2 globalTan(0.0f,0.0f);

	int collisionCount = 0;
	for (int w = 0 ; w < mNumStopWires; w++)
	{
	Wire * pWire = mStopWires[w];
	for (int i=0;i<pWire->numOfPoints-1 ; i++)
	{
	WirePoint & wA = pWire->wirePoints[i];
	WirePoint & wB = pWire->wirePoints[i+1];
	Line2D line;
	line.p0.x = wA.x;
	line.p0.y = wA.z;

	line.p1.x = wB.x;
	line.p1.y = wB.z;

	float dist = Collision::Vec2DistanceFromLine2D(line, nodeNewPos);
	if (dist < MM(mWalkDistance*1.1f))
	{

	PVRTVec2 interPoint = Collision::Vec2CollisionLine2D(line, nodeNewPos);
	//PVRTVec2 tangVec = nodeNewPos - interPoint;	
	PVRTVec2 tangVec(0.5f*(wA.tanx + wB.tanx), 0.5f*(wA.tanz + wB.tanz));
	//globalTan+=tangVec.normalized();

	//globalTan/=(float)collisionCount;
	//globalTan.normalize();

	float joyRotAngle = atan2f(-vecLeft.x, -vecLeft.y);
	float globalDirRotAngle = atan2f(tangVec.x, tangVec.y);

	if ( abs(globalDirRotAngle + joyRotAngle ) > PVRT_PI*0.6f)
	{
	colliding = false;
	}
	else
	{
	colliding = true;						
	}

	return colliding;
	}
	}
	}
	}
	*/
}

void PlayerNode::OnTick()
{
    
     JoyStick * pJoyStick = JoyStick::GetJoyStick();

	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();
	PVRTVec2 vecLeft = pJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = pJoyStick->GetVectorRight();


	mLeftJoy = vecLeft;
	mLeftJoy = (mLeftJoy / JOY_SCALE);
    

	bool colliding = CheckCollision();	
	
    
	PVRTQUATERNION quatLeft;
	PVRTQUATERNION quatRight;

	static float playerRotationAngle = 0.0f;

	float lenRight = vecRight.length();
	//if (lenRight > 0.1f || mSmoothRotate )
	{
		//vecRight*=1.0f/lenRight;
		
		//float angleFactor = 5.0f;
		//if (vecRight.x < -0.18f || vecRight.x > 0.18f)
			//angleFactor = 20.0f * vecRight.x;

        if ( mSmoothRotate )
            playerRotationAngle += 5 * lenRight * 1.0f * 0.017453292f * vecRight.x ;
        else
             playerRotationAngle += 5 * lenRight * 5.0f * 0.017453292f * vecRight.x ;
            
		PVRTMatrixRotationY(mRotMtxRight, playerRotationAngle );
		PVRTMatrixQuaternionRotationAxis(quatRight, PVRTVec3(0,1,0),playerRotationAngle);
        if (!mDontRotate)
        {
            mNode->SetRotation(quatRight.x , quatRight.y, quatRight.z, quatRight.w);
        }
	}



	if (!colliding)
	{
			
		PVRTVec4 vecRot( -mLeftJoy.x , 0, mLeftJoy.y, 1);
		vecRot = mRotMtxRight * vecRot;
		//vecRot = rotMtxLeft * vecRot;
		vecLeft.x = vecRot.x;
		vecLeft.y = vecRot.z;

        Shell * pShell = Shell::GetShell();
        float fpsFactor = pShell->GetFPSFactor();
		nodeWorldPos.x += mWalkDistance * vecLeft.x * fpsFactor;
		nodeWorldPos.z += mWalkDistance * vecLeft.y * fpsFactor;
	}

	
    if (!mDontWalk)
    {		
		mNode->SetTranslation(nodeWorldPos.x, nodeWorldPos.y, nodeWorldPos.z);
	}

	///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	///@@@@@@@@@@@@@@ This is for updating gun_node position @@@@@@@@@@@@@@
	///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
    mNode->Update();
    
	///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	///@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	//if (colliding)
	//	return;
	/*
	//Using LEFT ONLY:
	if (lengthLeft > 0.01f)
	{
	if (lengthLeft < 0.4f)
	{
	float rotAngle = atan2f(-vecLeft.x, -vecLeft.y);
	PVRTQUATERNION leftJoystickQuat;
	PVRTMatrixQuaternionRotationAxis(leftJoystickQuat, PVRTVec3(0,1,0),rotAngle);

	PVRTMATRIX rotMtxTmp;
	PVRTMatrixRotationQuaternion(rotMtxTmp, leftJoystickQuat);
	//PVRTMatrixRotationY(rotMtxTmp, rotAngle);
	PVRTMat4 rotMtx(rotMtxTmp.f); 

	PVRTVec4 walkVec;
	walkVec.x = 0;
	walkVec.y = 0;
	walkVec.z = -lengthLeft;
	walkVec.w = 1;
	walkVec *= rotMtx;

	nodeWorldPos.x += mWalkDistance * vecLeft.x;
	nodeWorldPos.z -= mWalkDistance * vecLeft.y;
	}
	}

	mNode->SetTranslation(nodeWorldPos.x, nodeWorldPos.y, nodeWorldPos.z);
	*/

	/*
	if (mNumTriggerWires > 0)
	{
	PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);

	bool colliding = false;
	for (int w = 0 ; w < mNumTriggerWires; w++)
	{
	Wire * pWire = mTriggerWires[w];
	for (int i=0;i<pWire->numOfPoints-1 ; i++)
	{
	WirePoint & wA = pWire->wirePoints[i];
	WirePoint & wB = pWire->wirePoints[i+1];
	Line2D line;
	line.p0.x = wA.x;
	line.p0.y = wA.z;

	line.p1.x = wB.x;
	line.p1.y = wB.z;

	float dist = Collision::Vec2DistanceFromLine2D(line, nodeNewPos);
	if (dist<MM(0.1f))
	{
	// Report to game logic
	break;

	}
	}
	}		
	}
	*/
}


void PlayerNode::AddTriggerWire(Wire * pWire)
{
	mTriggerWires[mNumTriggerWires] = pWire;
	mNumTriggerWires++;
	if (mNumTriggerWires == MAX_TRIGGER_WIRES)
	{
		LOGE("mNumTriggerWires == MAX_TRIGGER_WIRES");
		STOP;
	}
}

void PlayerNode::RemoveTriggerWire(Wire * pWire)
{
	for (int n=0;n<mNumTriggerWires;n++)
	{
		if (mTriggerWires[n] == pWire)
		{
			mNumTriggerWires--;
			mTriggerWires[n] = mTriggerWires[mNumTriggerWires];
			break;
		}
	}
}