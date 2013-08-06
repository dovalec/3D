#include "GunNode.h"
#include "BombNode.h"

NEWDEL_IMPL(GunNode)
GunNode::GunNode()
{
	mPlayerNode = NULL;
	mModelData = NULL;
	mLocator = NULL;
	mBulletManager = new BulletManager();
	mShootingRate = 10;
	mShootCount = 0;
	mUnderAttackCount = 0;

	mState = state_init;

}

GunNode::~GunNode()
{
	delete mBulletManager;
}

void GunNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
}

void GunNode::SetPlayerNode(PlayerNode * pPlayerNode)
{
	mPlayerNode = pPlayerNode;
}

void GunNode::SetLocator(Locator * pLocator)
{
	mLocator = pLocator;
}

void GunNode::SetShootingRate(int rate)
{
	mShootingRate = rate;
}

void GunNode::ExplodeGun()
{
	mState = state_exploded;
}

void GunNode::HandleBullet()
{
	if (mState == state_shoot)
		mUnderAttackCount++;
}

void GunNode::OnTick()
{
	switch (mState)
	{
		case state_init:
			InitState();
			break;
		case state_shoot:
			ShootState();
			break;
		case state_idle:
			IdleState();		
			break;
		case state_exploded:
			ExplodedState();
			break;	
		case state_finished:
			FinishedState();
			break;	
	}
	if (mState == state_shoot  && mUnderAttackCount == 10)
	{
		ExplodeGun();
		return;
	}

	if (mState == state_shoot || mState == state_idle )
	{
		PVRTVec3 playerWorldPos = mPlayerNode->GetNode()->GetWorldTranslation();
		PVRTVec3 locatorPos(mLocator->x, mLocator->y, mLocator->z);
		PVRTVec3 distVec = playerWorldPos - locatorPos;
		if (distVec.lenSqr() < MM(mLocator->radius))
		{
			mState = state_shoot;
			mShootCount++;
		}
		else
		{
			mState = state_idle;
			mShootCount = 0;
		}

		mBulletManager->OnTick();
	}
}



