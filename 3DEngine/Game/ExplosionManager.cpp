#include "ExplosionManager.h"

NEWDEL_IMPL(ExplosionManager)
ExplosionManager::ExplosionManager()
{
	mNumExplosionNodes = 0;
	mNumEnabledExplosions = 0;
}

ExplosionManager::~ExplosionManager()
{
	for (int n=0;n<mNumExplosionNodes;n++)
	{
		delete mExplosionNodes[n];
	}
}

void ExplosionManager::AddExplosionNode(ExplosionNode * pExplosionNode)
{
	if (mNumExplosionNodes == MAX_NUM_EXPLOSIONS)
	{
		LOGE("mNumExplosionNodes == MAX_NUM_EXPLOSIONS");
		STOP;
	}

	mExplosionNodes[mNumExplosionNodes] = pExplosionNode;
	mNumExplosionNodes++;
}


ExplosionNode * ExplosionManager::EnableExplosionNode()
{
	if (mNumEnabledExplosions == mNumExplosionNodes)
		return NULL;

	ExplosionNode * pExplosionNode = mExplosionNodes[mNumEnabledExplosions];
	if (pExplosionNode->IsEnabled())
		return NULL;

	pExplosionNode->Enable();
	mNumEnabledExplosions++;
	return pExplosionNode;
}

void ExplosionManager::DisableExplosionNode(int index)
{
	mExplosionNodes[index]->Disable();
	mNumEnabledExplosions--;
	ExplosionNode *pTmp = mExplosionNodes[mNumEnabledExplosions];
	mExplosionNodes[mNumEnabledExplosions] = mExplosionNodes[index];
	mExplosionNodes[index] = pTmp;
}


void ExplosionManager::RemoveExplosionNode(ExplosionNode * pExplosionNode)
{
	for (int n=0;n<mNumExplosionNodes;n++)
	{
		if (mExplosionNodes[n] == pExplosionNode)
		{
			mNumExplosionNodes--;
			mExplosionNodes[n] = mExplosionNodes[mNumExplosionNodes];
			break;
		}
	}
}

void ExplosionManager::RemoveExplosionNode(int index)
{
	mNumExplosionNodes--;
	mExplosionNodes[index] = mExplosionNodes[mNumExplosionNodes];
}

void ExplosionManager::OnTick()
{
	for (int n=0;n<mNumEnabledExplosions;n++)
	{
		mExplosionNodes[n]->OnTick();
		if (mExplosionNodes[n]->GetCurrentState() == ExplosionNode::state_finished)
		{
			DisableExplosionNode(n);
		}
	}
}

