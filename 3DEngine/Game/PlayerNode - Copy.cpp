#include "PlayerNode.h"
#include "../AI/Collision.h"
#include "../Shell.h"
#include "../CameraManager.h"

NEWDEL_IMPL(PlayerNode)
PlayerNode::PlayerNode()
{
	mJoyStick = NULL;
	mWalkDistance = 0.0f;
	mNumTriggerWires = 0;
	mModelData = NULL;
	mNode = NULL;

}

PlayerNode::~PlayerNode()
{
	
}
void PlayerNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
	mNode = mModelData->GetRoot();
}

void PlayerNode::SetJoyStick(JoyStick * pJoyStick)
{
	mJoyStick = pJoyStick;
}

void PlayerNode::SetWalkDistance(float dist)
{
	mWalkDistance = dist;
}

float PlayerNode::GetWalkDistance()
{
	return mWalkDistance;
}

void PlayerNode::OnTick()
{
	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();
	PVRTVec2 vecLeft = mJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = mJoyStick->GetVectorRight();
	
	float lengthLeft = vecLeft.length();

	//vecRight.x*=-1.0f;
	//vecLeft.x*=-1.0f;

		
	float lengthRight = vecRight.length();

	//Using LEFT ONLY:
	if (lengthLeft > 0.01f)
	{
		float rotAngle = atan2f(-vecLeft.x, -vecLeft.y);
		PVRTQUATERNION leftJoystickQuat;
		
		PVRTMatrixQuaternionRotationAxis(leftJoystickQuat, PVRTVec3(0,1,0),rotAngle);
		mNode->SetRotation(leftJoystickQuat.x , leftJoystickQuat.y, leftJoystickQuat.z, leftJoystickQuat.w);

		if (lengthLeft > 0.1f && lengthLeft < 0.4f)
		{
			if (mModelData->GetModelAnimations().GetCurrentClip()->GetSpeed() == 0.0f)
			{
				mModelData->GetModelAnimations().GetCurrentClip()->SetSpeed(1.0f);
				mModelData->GetModelAnimations().GetCurrentClip()->ResetAnimationPos();
			}

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
		else
		{
			if (mModelData->GetModelAnimations().GetCurrentClip()->GetSpeed() == 1.0f)
			{
				mModelData->GetModelAnimations().GetCurrentClip()->SetSpeed(0.0f);
				mModelData->GetModelAnimations().GetCurrentClip()->ResetAnimationPos();
			}
		}
	}

	if (mNumStopWires == 0)
	{
		mNode->SetTranslation(nodeWorldPos);
	}
	else
	{
		
		PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);
		
		bool colliding = false;
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
				if (dist < mWalkDistance*1.1f )
				{
					PVRTVec2 interPoint = Collision::Vec2CollisionLine2D(line, nodeNewPos);
					//PVRTVec2 distVec = nodeNewPos - interPoint;
					PVRTVec2 tangVec(0.5f*(wA.tanx + wB.tanx), 0.5f*(wA.tanz + wB.tanz));
					//distVec.normalize();
					PVRTVec2 vecLeftNormalized = vecLeft.normalized();
					//float dot = distVec.dot(vecLeftNormalized);
					float dot = tangVec.dot(vecLeftNormalized);

					if (abs(dot) > 0.01f)
						colliding = true;
					else
					{
						colliding = false;
					}
					//mNode->SetTranslation(interPoint.x, nodeWorldPos.y, interPoint.y);
					//colliding = true;
					break;
					
				}
				if (colliding)
					break;

			}
		}
		if (!colliding)
		{
			mNode->SetTranslation(nodeWorldPos);
		}
	}
	
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
				if (dist<0.1f)
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