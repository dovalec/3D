#ifndef __BulletsToNodesCollision__
#define __BulletsToNodesCollision__

#include "../Config.h"
#include "NodesToNodesCollision.h"
#include "BulletNode.h"


class BulletsToNodesCollision : public NodesToNodesCollision {
public:
	NEWDEL_DECL
	BulletsToNodesCollision(String type);
	virtual ~BulletsToNodesCollision();

	void AddBulletNode(BulletNode * pBullet);
	
	virtual void OnTick();

protected: 
	int mNumBullets;
	BulletNode * mBullets[MAX_COLLISION_OBJ_NUM];
};

#endif //__BulletsToNodesCollision__
