#include "GrenadesToNodesCollision.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(GrenadesToNodesCollision)
GrenadesToNodesCollision::GrenadesToNodesCollision(String type) : NodesToNodesCollision( type )
{
	mNumGrenades = 0;
}

GrenadesToNodesCollision::~GrenadesToNodesCollision()
{


}

void GrenadesToNodesCollision::AddGrenadeNode(GrenadeNode * pGrenade)
{
	mGrenades[mNumGrenades] = pGrenade;
	mNumGrenades++;
}


void GrenadesToNodesCollision::OnTick()
{

	int colCount = 0;

	for (int b=0;b<mNumGrenades;b++)
	{
		GrenadeNode * pGrenade = mGrenades[b];

		GrenadeNode::GrenadeState state = pGrenade->GetCurrentState();

		Node * pGrenadeRootNode = pGrenade->GetModelData()->GetRoot();
		
        if (    state == GrenadeNode::state_exploding &&
                pGrenadeRootNode->IsVisible() &&
                pGrenadeRootNode->IsInFrustum()
            )
		{	
			for (int n=0;n<mNumEventNodes;n++)
			{				
				CollisionEvent * pNodeEvent = mNodeEvents[n];
				Node * pNode = pNodeEvent->pCollisionNode->pNode;
                //if ( pNode->IsInFrustum() )
                //{
                //    LOGI("%x in frustum",(unsigned int)pNode);
                //}
                
				if (    
                        pNodeEvent->pCollisionNode->IsEnabled() &&                        
                        pNode->IsVisible() &&
                        pNode->IsInFrustum()  
                    )
				{
					//float hitDist = pGrenade->GetHitDistance();
					PVRTVec3 GrenadePos = pGrenade->GetGrenadePos();
					PVRTVec3 nodePos = pNode->GetWorldTranslation();
					nodePos.y = GrenadePos.y;
					PVRTVec3 distVec = nodePos - GrenadePos;
					if (distVec.lenSqr() < MM(pNodeEvent->minDist))
					{
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


