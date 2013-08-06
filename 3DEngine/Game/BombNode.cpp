#include "BombNode.h"

NEWDEL_IMPL(BombNode)

BombNode::BombNode() 
{
	// in case targeting a bomb to a specific node in scene - can be done by bot only!
	mTargetNode = NULL;
	
	//could be bot or playernode
	mShooterNode = NULL;

	mModelData = NULL;
	
	mState = state_idle;
	
	mHitDist = GLOBAL_SCALE*5.0f;

	mEnabled = false;

	mPower = 5;
	mHit = false;
}

BombNode::~BombNode()
{

}
void BombNode::SetBombHitFlag()
{
	mHit = true;
}

void BombNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}


void BombNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
	mModelData->GetRoot()->SetVisible(false);
}

void BombNode::SetLocator(Locator * pLocator)
{
	mLocator = pLocator;
}

void BombNode::SetShooterNode(Node * pNode)
{
	mShooterNode = pNode;
}

void BombNode::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}

void BombNode::SetTargetPos(PVRTVec3 pos)
{
	mTargetPos = pos;
}

void BombNode::SetBombPos(PVRTVec3 pos)
{
	mBombPos = pos;
}

void BombNode::SetAttachNode(Node * pNode)
{
	mAttachNode = pNode;
}

void BombNode::SetPower(int power)
{
	mPower = power;
}

int BombNode::GetPower()
{
	return mPower;
}


void BombNode::IdleState()
{
	
}

void BombNode::MovingState()
{
	mBombPos = mAttachNode->GetWorldTranslation();
	
	if (mTargetNode)
		mTargetPos = mTargetNode->GetWorldTranslation();	

	//auto explode by distance from target:
/*
	if (mAutoDistanceExplode())
	{
		PVRTVec3 distTargetBomb = mBombPos - mTargetPos;
		if (distTargetBomb.lenSqr() < MM(mHitDist))
		{
			mState = state_exploding;
		}
	}

	if (mAutoTimerExplode())
	{
		
		if (mTimePassed > mExplodeTimer)
		{
			mState = state_exploding;
		}
	}
*/
}

void BombNode::ExplodingState()
{
	mState = state_finished;
}

void BombNode::FinishedStat()
{
	
}


void BombNode::OnTick()
{
	switch (mState)
	{
		case state_idle:
			IdleState();
			break;
		case state_moving:
 			MovingState();
			break;
		case state_exploding:
			ExplodingState();
			break;
		case state_finished:
			FinishedStat();
			break;

	}

	if (  mState == state_idle || mState == state_moving )
	{
		
	}
/*
	PVRTVec3 playerWorldPos = mPlayerNode->GetNode()->GetWorldTranslation();
	PVRTVec3 locatorPos(mLocator->x, mLocator->y, mLocator->z);
	PVRTVec3 distVec = playerWorldPos - locatorPos;
	if (distVec.lenSqr() <MM( mLocator->radius))
	{
		ShootState();
	}
	else
	{
		IdleState();
	}
*/
	mModelData->GetRoot()->SetTranslation(mBombPos);
}

void BombNode::Enable()
{
	mHit = false;
	mEnabled = true;
	mState = state_idle;
	mModelData->GetRoot()->SetVisible(true);
}

void BombNode::Disable()
{
	mEnabled = false;
	mModelData->GetRoot()->SetVisible(false);
}

