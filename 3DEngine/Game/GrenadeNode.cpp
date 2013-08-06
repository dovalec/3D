#include "GrenadeNode.h"
#include "../TLSF/Tlsf.h"
#include "../Animation/AnimationPlayer.h"

NEWDEL_IMPL(GrenadeNode)

GrenadeNode::GrenadeNode()
{
	mShooterNode = NULL;
	mDirectionNode = NULL;
	mTargetNode = NULL;
	mModelData = NULL;
	mState = state_shoot;
	mSpeed = GLOBAL_SCALE*2.0f;
	mRange = GLOBAL_SCALE*50.0f;

	mFriction = GLOBAL_SCALE*0.0f;
	mGravity = GLOBAL_SCALE*0.1f;
	mHitDist = GLOBAL_SCALE*0.5f;
	mEnabled = false;

	mPower = 5;

	mHit = false;
}

GrenadeNode::~GrenadeNode()
{

}

void GrenadeNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}
void GrenadeNode::SetGravity(float gravity)
{
	mGravity = gravity;
}

void GrenadeNode::SetSpeed(float speed)
{
	mSpeed = speed;
}

void GrenadeNode::SetShootingRange(float range)
{
	mRange = range;
}


void GrenadeNode::SetFriction(float friction)
{
	mFriction = friction;
}


void GrenadeNode::SetModelData(ModelData * pModelData  )
{
	mModelData = pModelData;
	mModelData->GetRoot()->SetVisible(false);  
}


void GrenadeNode::SetShooterNode(Node * pNode)
{
	mShooterNode = pNode;
}

void GrenadeNode::SetDirectionNode(Node * pNode)
{
	mDirectionNode = pNode;
}


void GrenadeNode::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}

void GrenadeNode::SetTargetPos(PVRTVec3 pos)
{
	mTargetPos = pos;
}


void GrenadeNode::SetPower(int power)
{
	mPower = power;
}

int GrenadeNode::GetPower()
{
	return mPower;
}

#define RAND_TABLE_SIZE 20
void GrenadeNode::SetupTarget()
{
			
	PVRTMATRIX weaponRotaionMtx = mDirectionNode->GetWorldMtx();
	
	PVRTMat4 mat(weaponRotaionMtx.f);
	PVRTVec4 zVec(0, 0 , mRange, 1.0f);
	zVec = mat * zVec;

	PVRTVec3 targetPos;
	
	
	targetPos.x = zVec.x;
	targetPos.y = zVec.y;
	targetPos.z = zVec.z;

	SetTargetPos(targetPos);
}

void GrenadeNode::ShootState()
{
	SetupTarget();

	mGrenadeStartingPos = mGrenadePos = mShooterNode->GetWorldTranslation();
	
	if (mTargetNode)
		mTargetPos = mTargetNode->GetWorldTranslation();	


	PVRTVec3 dir =  mGrenadePos - mTargetPos;
	mGrenadePos -= dir * 0.1f;

	float rotAngle = atan2f(dir.z, dir.x) + PVRT_PI*0.5f;


	PVRTQUATERNION shooterQuat;
	PVRTMatrixQuaternionRotationAxis(shooterQuat, PVRTVec3(0,1,0),rotAngle);

	PVRTMATRIX shooterRotaionMtx;
	PVRTMatrixRotationQuaternion(shooterRotaionMtx, shooterQuat);

	PVRTMat4 mat(shooterRotaionMtx.f);
	PVRTVec4 zVec(0, 0 , 1.0f, 1.0f);
	zVec = mat * zVec;


/*
	if (mAutoAim)
	{
		mDir = mGrenadePos - mTargetNode->GetWorldTranslation();
		mDir.normalize();
	}
*/	
	mDir.x = zVec.x;
	mDir.y = zVec.y;
	mDir.z = zVec.z;

	//mDir.z = 0;
	//mDir.x = -1;

	mModelData->GetRoot()->SetRotation(shooterQuat);

	mState = state_fly;

}

void GrenadeNode::FlyState()
{
	 mGrenadePos += mDir * mSpeed;
	 mGrenadePos.y -= mGravity;
	 mSpeed -= mFriction;
	

	 if (mSpeed < 0.0f || mGrenadePos.y < 0.0f)
	 {
		 mState = state_exploding;
	 }
	 else
	 {
		PVRTVec3 fromStart = mGrenadePos - mGrenadeStartingPos;
		float fromStartDist = fromStart.lenSqr();
		if (fromStartDist > MM(mRange))
		{
			mState = state_exploding;
		}
	 }
}

void GrenadeNode::ExplodingState()
{
    mState = state_finished;
}

void GrenadeNode::FinishedStat()
{
    
}


void GrenadeNode::OnTick()
{
	switch (mState)
	{
		case state_shoot:
			ShootState();
			break;
		case state_fly:
			FlyState();
			break;
		case state_exploding:
			ExplodingState();
			break;
		case state_finished:
			FinishedStat();
			break;

	}
/*
	PVRTVec3 playerWorldPos = mPlayerNode->GetNode()->GetWorldTranslation();
	PVRTVec3 locatorPos(mLocator->x, mLocator->y, mLocator->z);
	PVRTVec3 distVec = playerWorldPos - locatorPos;
	if (distVec.lenSqr() < mLocator->radius)
	{
		ShootState();
	}
	else
	{
		IdleState();
	}
*/
	mModelData->GetRoot()->SetTranslation(mGrenadePos);
}

void GrenadeNode::Enable()
{
	mHit = false;
	mEnabled = true;
	mState = state_shoot;
	mModelData->GetRoot()->SetVisible(true);
}

void GrenadeNode::Disable()
{
	mEnabled = false;
	mModelData->GetRoot()->SetVisible(false);
   
}

