#include "BulletManager.h"

NEWDEL_IMPL(BulletManager)
BulletManager::BulletManager()
{
	mNumBulletNodes = 0;
}

BulletManager::~BulletManager()
{
	
}

void BulletManager::AddBulletNode(BulletNode * pBulletNode)
{
	mBulletNodes[mNumBulletNodes] = pBulletNode;
	mNumBulletNodes++;
	if (mNumBulletNodes == MAX_NUM_BULLETS)
	{
		LOGE("mNumBulletNodes == MAX_NUM_BULLETS");
		STOP;
	}
}


void BulletManager::EnableBulletNode(int index)
{
	mBulletNodes[index]->Enable(true);
}

void BulletManager::DisableBulletNode(int index)
{
	mBulletNodes[index]->Disable(true);
}


void BulletManager::RemoveBulletNode(BulletNode * pBulletNode)
{
	for (int n=0;n<mNumBulletNodes;n++)
	{
		if (mBulletNodes[n] == pBulletNode)
		{
			mBulletNodes[n] = mBulletNodes[mNumBulletNodes-1];
			mNumBulletNodes--;
			break;
		}
	}
}

void BulletManager::RemoveBulletNode(int index)
{
	mBulletNodes[index] = mBulletNodes[mNumBulletNodes-1];
	mNumBulletNodes--;
}

void BulletManager::Update()
{
	for (int n=0;n<mNumBulletNodes;n++)
	{
		mBulletNodes[n]->Update();
		if (mBulletNodes[n]->GetCurrentState() == BulletNode::state_finished)
		{
			RemoveBulletNode(n);
		}
	}
}


void BulletManager::Apply()
{
	for (int n=0;n<mNumBulletNodes;n++)
	{
		mBulletNodes[n]->Apply();
	}
}
