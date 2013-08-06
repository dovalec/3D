#include "BulletsToNodesCollision.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(BulletsToNodesCollision)
BulletsToNodesCollision::BulletsToNodesCollision(String type) : NodesToNodesCollision( type )
{
	mNumBullets = 0;
}

BulletsToNodesCollision::~BulletsToNodesCollision()
{


}

void BulletsToNodesCollision::AddBulletNode(BulletNode * pBullet)
{
	mBullets[mNumBullets] = pBullet;
	mNumBullets++;
}


void BulletsToNodesCollision::OnTick()
{

	int colCount = 0;

	for (int b=0;b<mNumBullets;b++)
	{
		BulletNode * pBullet = mBullets[b];

		BulletNode::BulletState state = pBullet->GetCurrentState();

		Node * pBulletRootNode = pBullet->GetModelData()->GetRoot();
		if (state == BulletNode::state_fly && pBulletRootNode->IsVisible() && pBulletRootNode->IsInFrustum())
		{	
			for (int n=0;n<mNumEventNodes;n++)
			{				
				CollisionEvent * pNodeEvent = mNodeEvents[n];
				Node * pNode = pNodeEvent->pCollisionNode->pNode;
                //if ( pNode->IsInFrustum() )
                //{
                //    LOGI("%x in frustum",(unsigned int)pNode);
                //}
                
				if ( pNodeEvent->pCollisionNode->IsEnabled() && pNode->IsVisible() && pNode->IsInFrustum() )
				{
					//float hitDist = pBullet->GetHitDistance();
					PVRTVec3 bulletPos = pBullet->GetBulletPos();
					PVRTVec3 nodePos = pNode->GetWorldTranslation();
					nodePos.y = bulletPos.y;
					PVRTVec3 distVec = nodePos - bulletPos;
					if (distVec.lenSqr() < MM(pNodeEvent->minDist))
					{
						pBullet->SetBulletHitFlag();
						
						EventManager & eventManager = EventManager::GetEventManager();
						eventManager.SendEventNoDestroy( pNodeEvent );
						colCount++;
						return;
					}
				}
	
			}			
		}
	}
}


