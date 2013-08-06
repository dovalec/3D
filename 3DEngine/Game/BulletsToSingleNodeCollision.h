#ifndef __BulletsToSingleNodeCollision__
#define __BulletsToSingleNodeCollision__

#include "../Config.h"
#include "BulletsToNodesCollision.h"
#include "BulletNode.h"


class BulletsToSingleNodeCollision : public BulletsToNodesCollision {
public:
	NEWDEL_DECL
	BulletsToSingleNodeCollision( String type );
	virtual ~BulletsToSingleNodeCollision();

	void SetNodeEvent(CollisionEvent * pNodeEvent);
	
	virtual void OnTick();


protected: 
	CollisionEvent * mNodeEvent;
};

#endif //__BulletsToSingleNodeCollision__
