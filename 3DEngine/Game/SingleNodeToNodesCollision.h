#ifndef __SingleNodeToNodesCollision__
#define __SingleNodeToNodesCollision__

#include "../Config.h"
#include "NodesToNodesCollision.h"

class SingleNodeToNodesCollision : public NodesToNodesCollision {
public:
	NEWDEL_DECL
	SingleNodeToNodesCollision(String type);
	virtual ~SingleNodeToNodesCollision();

	void SetCollisionNode(CollisionNode * pCollisionNode);
	
	virtual void OnTick();

protected: 
	CollisionNode * mCollisionNode;
};

#endif //__SingleNodeToNodesCollision__
