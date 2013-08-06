#ifndef __ExplosionManager__
#define __ExplosionManager__

#include "../Config.h"
#include "ExplosionNode.h"

class ExplosionManager {

public:
	NEWDEL_DECL
	ExplosionManager();
	virtual ~ExplosionManager();
	
	void OnTick();

	void AddExplosionNode(ExplosionNode * pExplosionNode);
	void RemoveExplosionNode(ExplosionNode * pExplosionNode);
	void RemoveExplosionNode(int index);

	ExplosionNode * EnableExplosionNode();
	void DisableExplosionNode(int index);


protected:
	int mNumExplosionNodes;
	int mNumEnabledExplosions;
	ExplosionNode * mExplosionNodes[MAX_NUM_EXPLOSIONS];
	
};

#endif //__ExplosionManager__
