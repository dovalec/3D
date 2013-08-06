#include "SingleNodeToNodesCollision.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(SingleNodeToNodesCollision)
SingleNodeToNodesCollision::SingleNodeToNodesCollision( String type ) : NodesToNodesCollision( type )
{
	mCollisionNode = NULL;
}

SingleNodeToNodesCollision::~SingleNodeToNodesCollision()
{
	delete mCollisionNode;
}

void SingleNodeToNodesCollision::SetCollisionNode(CollisionNode * pCollisionNode)
{
	mCollisionNode = pCollisionNode;
}

void SingleNodeToNodesCollision::OnTick()
{	
	if (mCollisionNode == NULL)
		return;

	if (mCollisionNode->pNode->IsVisible() && mCollisionNode->pNode->IsInFrustum())
	{
		PVRTVec3 nodePos1 = mCollisionNode->pNode->GetWorldTranslation();

		for (int e=0;e<mNumEventNodes;e++)
		{
			CollisionEvent * pNodeEvent = mNodeEvents[e];
			Node * pNode = pNodeEvent->pCollisionNode->pNode;

			if ( pNode->IsVisible() && pNode->IsInFrustum() )
			{			
				PVRTVec3 nodePos2 = pNode->GetWorldTranslation();
				nodePos1.y = nodePos2.y;
				PVRTVec3 distVec = nodePos1 - nodePos2;
				if (distVec.lenSqr() < MM(pNodeEvent->minDist))
				{
					//pNodeEvent->pCollisionNode = mCollisionNode;

					EventManager & eventManager = EventManager::GetEventManager();
					eventManager.SendEventNoDestroy( pNodeEvent );
					return;
				}
			}

		}
	}
}


