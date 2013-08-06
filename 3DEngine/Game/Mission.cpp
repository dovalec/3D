#include "BulletNode.h"
#include "../TLSF/Tlsf.h"

NEWDEL_IMPL(BulletNode)

BulletNode::BulletNode()
{
	mShooterNode = NULL;
	mTargetNode = NULL;
	mModelData = NULL;
	mState = state_shoot;
	mSpeed = 1.0f;
	mFriction = 0.0f;
	mGravity = 0.01f;
	mHitDist = 0.01f;
	mEnabled = false;

	mPower = 5;

}

BulletNode::~BulletNode()
{

}
/*
void * BulletNode::operator	new(unsigned int size)
{
	return HeapTlsf::GetTLSF().TLSF_NEW(size);
}

void   BulletNode::operator	delete(void * ptr)
{
	HeapTlsf::GetTLSF().TLSF_DEL(ptr);
}
*/

void BulletNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}
void BulletNode::SetSpeed(float speed)
{
	mSpeed = speed;
}

void BulletNode::SetFriction(float friction)
{
	mFriction = friction;
}


void BulletNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
}

void BulletNode::SetShooterNode(Node * pNode)
{
	mShooterNode = pNode;
}

void BulletNode::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}

void BulletNode::SetPower(int power)
{
	mPower = power;
}

int BulletNode::GetPower()
{
	return mPower;
}

void BulletNode::ShootState()
{
	mBulletPos = mShooterNode->GetWorldTranslation();
	mDir = mBulletPos - mTargetNode->GetWorldTranslation();
	mDir.normalize();

	mState = state_fly;

}

void BulletNode::FlyState()
{
	 mBulletPos += mDir * mSpeed;
	 mBulletPos.y -= mGravity;
	 mSpeed -= mFriction;

	 PVRTVec3 toTarget = mBulletPos - mTargetNode->GetWorldTranslation();
	 float dist = toTarget.length();
	 if (dist < mHitDist)
	 {
		mState = state_hit;
	 }
	 else if (mSpeed < 0.0f || mBulletPos.y < 0.0f)
	 {
		 mState = state_missed;
	 }
}

void BulletNode::HitState()
{

}

void BulletNode::MissedStat()
{

}


void BulletNode::Update()
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

	}
     */
/*
	PVRTVec3 playerWorldPos = mPlayerNode->GetNode()->GetWorldTranslation();
	PVRTVec3 locatorPos(mLocator->x, mLocator->y, mLocator->z);
	PVRTVec3 distVec = playerWorldPos - locatorPos;
	if (distVec.length() < mLocator->radius)
	{
		ShootState();
	}
	else
	{
		IdleStat();
	}
*/
	mModelData->GetRoot()->SetTranslation(mBulletPos);
	mModelData->GetRoot()->Update();
}

void BulletNode::Apply()
{
	mModelData->GetRoot()->Apply();
}


void BulletNode::Enable()
{
	mEnabled = true;
}

void BulletNode::Disable()
{
	mEnabled = false;
}

