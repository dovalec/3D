#include "GunManager.h"
#include "NodesToNodesCollision.h"

NEWDEL_IMPL(GunManager)
GunManager::GunManager() : EventSender("gun_manager")
{
	mNumGunNodes = 0;

	EventManager & eventManager = EventManager::GetEventManager();
	eventManager.AddEventReceiver(this);
}

GunManager::~GunManager()
{
	EventManager & eventManager = EventManager::GetEventManager();
	eventManager.RemoveEventReceiver(this);

	for ( int n=0;n<mNumGunNodes ; n++)
	{
		delete mGunNodes[mNumGunNodes];
	}
}

void GunManager::AddGunNode(GunNode * pGunNode)
{
	if (mNumGunNodes == MAX_NUM_GUNS)
	{
		LOGE("mNumGunNodes == MAX_NUM_GUNS");
		STOP;
	}

	mGunNodes[mNumGunNodes] = pGunNode;
	mNumGunNodes++;
}

void GunManager::RemoveGunNode(GunNode * pGunNode)
{
	for (int n=0;n<mNumGunNodes;n++)
	{
		if (mGunNodes[n] == pGunNode)
		{
			mNumGunNodes--;
			mGunNodes[n] = mGunNodes[mNumGunNodes];
			break;
		}
	}
}


void GunManager::OnTick()
{
	for (int n=0;n<mNumGunNodes;n++)
	{
		mGunNodes[n]->OnTick();
	}
}


void GunManager::HandleEvent(Event * pEvent)
{
	if (pEvent->GetType() == "event_collision_bombs_guns")
	{
		CollisionEvent * pNodeEvent = (CollisionEvent*)pEvent;
		GunNode * pGunNode = (GunNode*)pNodeEvent->pExtra1;
		if ( pGunNode->GetCurrentState() == GunNode::state_shoot)
		{
			pGunNode->ExplodeGun();
			EventManager & eventManager = EventManager::GetEventManager();
			eventManager.SendEvent(new Event(this, "event_bomb_explosion"));

		}
	}
}
