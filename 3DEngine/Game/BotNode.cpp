#include "BotNode.h"
#include "../TLSF/Tlsf.h"
#include "BotFsmMachine.h"

NEWDEL_IMPL(BotNode)

BotNode::BotNode()
{
	mModelData = NULL;
	mPlayerNode = NULL;
	mBotManager = NULL;

	mSpeed = GLOBAL_SCALE*2.0f;
	mRange = GLOBAL_SCALE*50.0f;

	mHitDist = GLOBAL_SCALE*0.01f;
	mEnabled = false;

	mPower = 5;
    
    mOnlineMode = false;
   
    mNetWalkVec *= 0.0f;
    mNewNetTranslation*=0.0f;
    mPrevNetTranslation*=0.0f;
    PVRTMatrixQuaternionIdentity(mNetRotation);
    
    mNetDeltaTick = 0;
    mNetTickCount  = 0;
}

BotNode::~BotNode()
{
	delete mMachine;
}


void BotNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}

void BotNode::SetSpeed(float speed)
{
	mSpeed = speed;
}

void BotNode::SetShootingRange(float range)
{
	mRange = range;
}

void BotNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
}

void BotNode::SetPower(int power)
{
	mPower = power;
}

int BotNode::GetPower()
{
	return mPower;
}

void BotNode::SetMachine(BotFsmMachine * pMachine)
{
	mMachine = pMachine;
	mMachine->SetBotNode(this);
}

void BotNode::OnTick()
{
	
}


void BotNode::Enable()
{
	mEnabled = true;
	//mState = state_shoot;
}

void BotNode::Disable()
{
	mEnabled = false;
}

void BotNode::SetBotManager(BotManager * pBotManager)
{
	mBotManager = pBotManager;	
}

void BotNode::SetPlayerNode(PlayerNode * pPlayerNode)
{
	mPlayerNode = pPlayerNode;
}
