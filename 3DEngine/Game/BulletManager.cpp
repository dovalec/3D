#include "BulletManager.h"

NEWDEL_IMPL(BulletManager)
BulletManager::BulletManager()
{
	mEnabled = true;
}

BulletManager::~BulletManager()
{
	ListIterator iter(mActiveBullets);
	while (BulletNode * pBullet = (BulletNode*)iter.GetPtr())
	{
		delete pBullet;
	}
	
}

void BulletManager::AddBulletNode(BulletNode * pBulletNode)
{
	mIdleBullets.Add(pBulletNode);
}


BulletNode * BulletManager::EnableBulletNode()
{
	BulletNode * pBullet = (BulletNode*)mIdleBullets.First();
	if (pBullet == NULL)
		return NULL;

	pBullet->Enable();

	ListItem * pItem = mIdleBullets.RemoveFirstOptimized();
	pBullet->mListItem = pItem;
	mActiveBullets.AddOptimized(pBullet, pItem);

	return pBullet;
}

void BulletManager::Enable()
{
	mEnabled = true;

	ListIterator iter(mActiveBullets);
	while (BulletNode * pBullet = (BulletNode*)iter.GetPtr())
	{
		pBullet->Enable();
	}
}

void BulletManager::Disable()
{
	ListIterator iter(mActiveBullets);
	while (BulletNode * pBullet = (BulletNode*)iter.GetPtr())
	{
		DisableBulletNode(pBullet);
	}
}

void BulletManager::DisableBulletNode(BulletNode * pBullet)
{
	mActiveBullets.RemoveOptimized(pBullet->mListItem);
	mIdleBullets.AddOptimized(pBullet, pBullet->mListItem );

	pBullet->Disable();
}


void BulletManager::OnTick()
{
	if (!mEnabled)
		return;

	ListIterator iter(mActiveBullets);
	while (BulletNode * pBullet = (BulletNode*)iter.GetPtr())
	{
		pBullet->OnTick();
		BulletNode::BulletState state = pBullet->GetCurrentState();

		switch (state)
		{
			case BulletNode::state_finished:
				DisableBulletNode(pBullet);
				break;

			case BulletNode::state_hit:		
				DisableBulletNode(pBullet);
				break;
		}
	}
	
}


