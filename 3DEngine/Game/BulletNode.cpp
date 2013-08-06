#include "BulletNode.h"
#include "../TLSF/Tlsf.h"

NEWDEL_IMPL(BulletNode)

BulletNode::BulletNode()
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
    mNoisy = false;
}

BulletNode::~BulletNode()
{

}
void BulletNode::SetBulletHitFlag()
{
	mHit = true;
}

void BulletNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}
void BulletNode::SetGravity(float gravity)
{
	mGravity = gravity;
}

void BulletNode::SetSpeed(float speed)
{
	mSpeed = speed;
}

void BulletNode::SetShootingRange(float range)
{
	mRange = range;
}


void BulletNode::SetFriction(float friction)
{
	mFriction = friction;
}


void BulletNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
	mModelData->GetRoot()->SetVisible(false);
}

void BulletNode::SetShooterNode(Node * pNode)
{
	mShooterNode = pNode;
}

void BulletNode::SetDirectionNode(Node * pNode)
{
	mDirectionNode = pNode;
}


void BulletNode::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}

void BulletNode::SetTargetPos(PVRTVec3 pos)
{
	mTargetPos = pos;
}


void BulletNode::SetPower(int power)
{
	mPower = power;
}

int BulletNode::GetPower()
{
	return mPower;
}

#define RAND_TABLE_SIZE 20
void BulletNode::SetupTarget()
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

void BulletNode::ShootState()
{
	SetupTarget();

	mBulletStartingPos = mBulletPos = mShooterNode->GetWorldTranslation();
	
	if (mTargetNode)
		mTargetPos = mTargetNode->GetWorldTranslation();	


	PVRTVec3 dir =  mBulletPos - mTargetPos;
	mBulletPos -= dir * 0.1f;

	float rotAngle = atan2f(dir.z, dir.x) + PVRT_PI*0.5f;


	static int randIndex = 0;
	const static float randTable[RAND_TABLE_SIZE] = {1,-4,3,-2,5,-3,6,-5,2,-3,4,-2,4,-5,5,-3,4,-5,6,-3};
	if (mNoisy)
    {
        rotAngle = rotAngle + 0.25f * randTable[randIndex++ % RAND_TABLE_SIZE] * (0.017453292f);
	}

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
		mDir = mBulletPos - mTargetNode->GetWorldTranslation();
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

void BulletNode::FlyState()
{
	 mBulletPos += mDir * mSpeed;
	 mBulletPos.y -= mGravity;
	 mSpeed -= mFriction;
	
	 if (mHit)
	 {
		mState = state_hit;
		return;
	 }

	 if (mSpeed < 0.0f || mBulletPos.y < 0.0f)
	 {
		 mState = state_missed;
	 }
	 else
	 {
		PVRTVec3 fromStart = mBulletPos - mBulletStartingPos;
		float fromStartDist = fromStart.lenSqr();
		if (fromStartDist > MM(mRange))
		{
			mState = state_missed;
		}
	 }
}

void BulletNode::HitState()
{
	mState = state_finished;
}

void BulletNode::MissedStat()
{
	mState = state_finished;
}

void BulletNode::FinishedStat()
{
    mNoisy = false;
}


void BulletNode::OnTick()
{
	switch (mState)
	{
		case state_shoot:
			ShootState();
			break;
		case state_fly:
			FlyState();
			break;
		case state_hit:
			HitState();
			break;
		case state_missed:
			MissedStat();
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
	mModelData->GetRoot()->SetTranslation(mBulletPos);
}

void BulletNode::Enable()
{
	mHit = false;
	mEnabled = true;
	mState = state_shoot;
	mModelData->GetRoot()->SetVisible(true);
}

void BulletNode::Disable()
{
	mEnabled = false;
	mModelData->GetRoot()->SetVisible(false);
}

