#include "BulletsToSingleNodeCollision.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(BulletsToSingleNodeCollision)
BulletsToSingleNodeCollision::BulletsToSingleNodeCollision( String type) : BulletsToNodesCollision( type )
{
	mNodeEvent = NULL;
}

BulletsToSingleNodeCollision::~BulletsToSingleNodeCollision()
{
	delete mNodeEvent;
}

void BulletsToSingleNodeCollision::SetNodeEvent(CollisionEvent * pNodeEvent)
{
	mNodeEvent = pNodeEvent;
}


void BulletsToSingleNodeCollision::OnTick()
{
	for (int b=0;b<mNumBullets;b++)
	{
		BulletNode * pBullet = mBullets[b];

		Node * pBulletRootNode = pBullet->GetModelData()->GetRoot();
		
		BulletNode::BulletState state = pBullet->GetCurrentState();
		
		if (state == BulletNode::state_fly && pBulletRootNode->IsVisible() && pBulletRootNode->IsInFrustum() )		
		{	

			Node * pNode = mNodeEvent->pCollisionNode->pNode;

			if ( pNode->IsVisible() && pNode->IsInFrustum() )
			{
				//float hitDist = pBullet->GetHitDistance();
				PVRTVec3 bulletPos = pBullet->GetBulletPos();
				PVRTVec3 nodePos = pNode->GetWorldTranslation();
				nodePos.y = bulletPos.y;
				PVRTVec3 distVec = nodePos - bulletPos;
                float distLen = distVec.lenSqr();
               // LOGI("Dist: %f",distLen);
                
				if ( distLen < MM(mNodeEvent->minDist))
				{
					pBullet->SetBulletHitFlag();
					
					EventManager & eventManager = EventManager::GetEventManager();
					eventManager.SendEventNoDestroy( mNodeEvent );
					return;
				}
			}			
		}
	}
}


