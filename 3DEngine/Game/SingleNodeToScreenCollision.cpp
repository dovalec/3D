
#include "SingleNodeToScreenCollision.h"
#include "../Events/EventManager.h"


NEWDEL_IMPL(SingleNodeToScreenCollision)
SingleNodeToScreenCollision::SingleNodeToScreenCollision( String type ) : INodesCollision( type )
{
	mNodeEvent = NULL;
}

SingleNodeToScreenCollision::~SingleNodeToScreenCollision()
{
	delete mNodeEvent;
}

void SingleNodeToScreenCollision::SetNodeEvent(CollisionEvent * pNodeEvent)
{
	mNodeEvent = pNodeEvent;
}

CollisionEvent * SingleNodeToScreenCollision::GetNodeEvent()
{
	return mNodeEvent;
}


void SingleNodeToScreenCollision::OnTick()
{	
	Node * pNode = mNodeEvent->pCollisionNode->pNode;

	float touchPosX;
	float touchPosY;

	ScreenTouch & touch =  ScreenTouch::GetScreenTouch();

	if (touch.GetTouch(touchPosX, touchPosY))
	{
		if (pNode->IsVisible() && pNode->IsInFrustum())
		{
			PVRTVec2 screenPos = pNode->GetProjected();
			

			PVRTVec2 touchPos(touchPosX, touchPosY);
			PVRTVec2 distVec = screenPos-touchPos;
			if (distVec.lenSqr()  < MM( mNodeEvent->minDist ) )
			{
				EventManager & eventManager = EventManager::GetEventManager();
				eventManager.SendEventNoDestroy( mNodeEvent );
				return;
				
			}
		}
	}
}


