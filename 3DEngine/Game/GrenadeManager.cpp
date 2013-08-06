#include "GrenadeManager.h"

NEWDEL_IMPL(GrenadeManager)
GrenadeManager::GrenadeManager()
{
	mEnabled = true;
}

GrenadeManager::~GrenadeManager()
{
	ListIterator iter(mActiveGrenades);
	while (GrenadeNode * pGrenade = (GrenadeNode*)iter.GetPtr())
	{
		delete pGrenade;
	}
	
}

void GrenadeManager::AddGrenadeNode(GrenadeNode * pGrenadeNode)
{
	mIdleGrenades.Add(pGrenadeNode);
}


GrenadeNode * GrenadeManager::EnableGrenadeNode()
{
	GrenadeNode * pGrenade = (GrenadeNode*)mIdleGrenades.First();
	if (pGrenade == NULL)
		return NULL;

	pGrenade->Enable();

	ListItem * pItem = mIdleGrenades.RemoveFirstOptimized();
	pGrenade->mListItem = pItem;
	mActiveGrenades.AddOptimized(pGrenade, pItem);

	return pGrenade;
}

void GrenadeManager::Enable()
{
	mEnabled = true;

	ListIterator iter(mActiveGrenades);
	while (GrenadeNode * pGrenade = (GrenadeNode*)iter.GetPtr())
	{
		pGrenade->Enable();
	}
}

void GrenadeManager::Disable()
{
	ListIterator iter(mActiveGrenades);
	while (GrenadeNode * pGrenade = (GrenadeNode*)iter.GetPtr())
	{
		DisableGrenadeNode(pGrenade);
	}
}

void GrenadeManager::DisableGrenadeNode(GrenadeNode * pGrenade)
{
	mActiveGrenades.RemoveOptimized(pGrenade->mListItem);
	mIdleGrenades.AddOptimized(pGrenade, pGrenade->mListItem );

	pGrenade->Disable();
}


void GrenadeManager::OnTick()
{
	if (!mEnabled)
		return;

	ListIterator iter(mActiveGrenades);
	while (GrenadeNode * pGrenade = (GrenadeNode*)iter.GetPtr())
	{
		pGrenade->OnTick();
		GrenadeNode::GrenadeState state = pGrenade->GetCurrentState();

		switch (state)
		{
			case GrenadeNode::state_finished:
				DisableGrenadeNode(pGrenade);
				break;
		}
	}
	
}


