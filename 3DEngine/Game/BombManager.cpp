#include "BombManager.h"
#include "GunNode.h"

NEWDEL_IMPL(BombManager)
BombManager::BombManager()
{
	mNumBombNodes = 0;
	mNumEnabledBombs = 0;

	mCollisionManager.CreateCollision(CollisionManager::collision_single_node_to_nodes, "sender_bomb_manager", (int) collision_shooter_bombs);
	mCollisionManager.CreateCollision(CollisionManager::collision_nodes_to_nodes, "sender_bomb_manager", (int) collision_bombs_targets);

	EventManager & eventManager = EventManager::GetEventManager();
	eventManager.AddEventReceiver(this);
}


BombManager::~BombManager()
{
	EventManager & eventManager = EventManager::GetEventManager();
	eventManager.RemoveEventReceiver(this);

	for (int n=0;n < mNumBombNodes ; n++)
	{
		delete mBombNodes[n];
	}
}

void BombManager::AddBombNode(BombNode * pBombNode)
{
	if (mNumBombNodes == MAX_NUM_BOMBS)
	{
		LOGE("mNumBombNodes == MAX_NUM_BOMBS");
		STOP;
	}

	mBombNodes[mNumBombNodes] = pBombNode;
	mNumBombNodes++;

}

void BombManager::EnableAllBombs()
{
	mNumEnabledBombs = mNumBombNodes;
	for (int n=0;n<mNumBombNodes;n++)
	{
		mBombNodes[n]->Enable();
	}
}

BombNode * BombManager::EnableBombNode()
{
	if (mNumEnabledBombs == mNumBombNodes)
		return NULL;

	BombNode * pBombNode = mBombNodes[mNumEnabledBombs];
	if (pBombNode->IsEnabled())
		return NULL;

	pBombNode->Enable();
	mNumEnabledBombs++;
	return pBombNode;
}

void BombManager::DisableBombNode(int index)
{
	if (mNumEnabledBombs == 0)
	{
		STOP;
	}

	mBombNodes[index]->Disable();
	mNumEnabledBombs--;
	BombNode *pTmp = mBombNodes[mNumEnabledBombs];
	mBombNodes[mNumEnabledBombs] = mBombNodes[index];
	mBombNodes[index] = pTmp;
}

void BombManager::DisableBombNode(BombNode * pBombNode)
{
	int index = GetBombNodeIndex(pBombNode);
	DisableBombNode(index);
}



int BombManager::GetBombNodeIndex(BombNode * pBombNode)
{
	for (int n=0;n<mNumBombNodes;n++)
	{
		if (mBombNodes[n] == pBombNode)
			return n;
	}

	return -1;
}


void BombManager::RemoveBombNode(BombNode * pBombNode)
{
	int index = GetBombNodeIndex(pBombNode);
	RemoveBombNode(index);
}

void BombManager::RemoveBombNode(int index)
{
	mNumBombNodes--;
	mBombNodes[index] = mBombNodes[mNumBombNodes];
}

void BombManager::OnTick()
{	
	for (int n=0;n<mNumEnabledBombs;n++)
	{
		mBombNodes[n]->OnTick();
		BombNode::BombState state = mBombNodes[n]->GetCurrentState();
		switch (state)
		{
			case BombNode::state_finished:
				DisableBombNode(n);
				break;
		}
	}

	mCollisionManager.OnTick();

}


void BombManager::HandleEvent(Event * pEvent)
{
    const String & eventType = pEvent->GetType();
    
	
		//if ( eventType == String("event_collision_bombs_guns") )
			{
				CollisionEvent * pNodeEvent = (CollisionEvent*)pEvent;
				BombNode * pBomb = (BombNode*)pNodeEvent->pCollisionNode->pExtra1;			
				GunNode * pGunNode = (GunNode*)pNodeEvent->pExtra1;
				if ( pGunNode->GetCurrentState() != GunNode::state_finished)
				{
					if (pBomb->GetCurrentState() == BombNode::state_moving )
					{
						pBomb->SwitchToState(BombNode::state_exploding);								
					}
					else if (pBomb->GetCurrentState() == BombNode::state_finished )
					{
						DisableBombNode(pBomb);
					}
				}
			}
    
/*
		case EventEnums::event_collision_shooter_bombs :
			{
				CollisionEvent * pNodeEvent = (CollisionEvent*)pEvent;
				BombNode * pBomb = (BombNode*)pNodeEvent->pExtra1;
				if ( pBomb->GetAutoDistancePick() )
				{
					SingleNodeToNodesCollision *  collisionFarmerBombs = (SingleNodeToNodesCollision*)mCollisionManager.GetCollision( BombManager::collision_shooter_bombs);
					collisionFarmerBombs->Disable();
				}
			}
			break;
	
		case EventEnums::event_bomb_explosion :
			{
				SingleNodeToNodesCollision *  collisionFarmerBombs = (SingleNodeToNodesCollision*)mCollisionManager.GetCollision( BombManager::collision_shooter_bombs);
				collisionFarmerBombs->Enable();
			}
			break;
*/


}




