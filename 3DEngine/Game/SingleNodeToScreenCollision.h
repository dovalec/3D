#ifndef __SingleNodeToScreenCollision__
#define __SingleNodeToScreenCollision__

#include "../Config.h"
#include "../UI/ScreenTouch.h"
#include "NodesToNodesCollision.h"

class SingleNodeToScreenCollision : public INodesCollision {
public:
	NEWDEL_DECL
	SingleNodeToScreenCollision(String type);
	virtual ~SingleNodeToScreenCollision();

	void SetNodeEvent(CollisionEvent * pNodeEvent);
	CollisionEvent * GetNodeEvent();
	
	virtual void OnTick();

protected: 
	CollisionEvent * mNodeEvent;
};

#endif //__SingleNodeToScreenCollision__
