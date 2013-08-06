#ifndef __GrenadesToNodesCollision__
#define __GrenadesToNodesCollision__

#include "../Config.h"
#include "NodesToNodesCollision.h"
#include "GrenadeNode.h"


class GrenadesToNodesCollision : public NodesToNodesCollision {
public:
	NEWDEL_DECL
	GrenadesToNodesCollision(String type);
	virtual ~GrenadesToNodesCollision();

	void AddGrenadeNode(GrenadeNode * pGrenade);
	
	virtual void OnTick();

protected: 
	int mNumGrenades;
	GrenadeNode * mGrenades[MAX_COLLISION_OBJ_NUM];
};

#endif //__GrenadesToNodesCollision__
