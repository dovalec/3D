#include "NodesToNodesCollision.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(CollisionEvent)
NEWDEL_IMPL(CollisionNode)
NEWDEL_IMPL(NodesToNodesCollision)
NEWDEL_IMPL(INodesCollision)
INodesCollision::INodesCollision( String type ) : EventSender( type )
{
}
INodesCollision::~INodesCollision()
{
}
NodesToNodesCollision::NodesToNodesCollision( String type ) : INodesCollision( type )
{
	mNumNodes = 0;
	mNumEventNodes  = 0;
}

NodesToNodesCollision::~NodesToNodesCollision()
{
	for (int n=0;n<mNumNodes;n++)
	{
		delete mCollisionNodes[n];
	}

	for (int n=0;n<mNumEventNodes;n++)
	{
		delete mNodeEvents[n];
	}
}

void NodesToNodesCollision::AddCollisionNode(CollisionNode * pCollisionNode)
{
	mCollisionNodes[mNumNodes] = pCollisionNode;
	mNumNodes++;
}

void NodesToNodesCollision::AddNodeEvent(CollisionEvent * pNodeEvent)
{
	mNodeEvents[mNumEventNodes] = pNodeEvent;
	mNumEventNodes++;
}


void NodesToNodesCollision::OnTick()
{
	for (int b=0;b<mNumNodes;b++)
	{
		CollisionNode * pCollisionNodeLocal = mCollisionNodes[b];

		for (int e=0;e<mNumEventNodes;e++)
		{
			CollisionEvent * pNodeEvent = mNodeEvents[e];
			Node * pNode = pNodeEvent->pCollisionNode->pNode;

			if ( pCollisionNodeLocal->pNode->IsVisible() && pCollisionNodeLocal->pNode->IsInFrustum() && 
				pNode->IsVisible() && pNode->IsInFrustum() )
			{
			
				PVRTVec3 nodePos1 = pCollisionNodeLocal->pNode->GetWorldTranslation();
				PVRTVec3 nodePos2 = pNode->GetWorldTranslation();
				nodePos1.y = nodePos2.y;
				PVRTVec3 distVec = nodePos1 - nodePos2;
				if (distVec.lenSqr() < MM(pNodeEvent->minDist))
				{
					//pNodeEvent->pCollisionNode = pCollisionNodeLocal;
					EventManager & eventManager = EventManager::GetEventManager();
					eventManager.SendEventNoDestroy( pNodeEvent );
					return;
				}
			}

		}
	}
}


