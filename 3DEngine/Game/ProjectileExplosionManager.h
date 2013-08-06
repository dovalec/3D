#ifndef __ProjectileExplosionManager__
#define __ProjectileExplosionManager__

#include "../Config.h"
#include "ProjectileExplosionNode.h"

#define MAX_NUM_PROJECTILE_EXPLOSIONS 32

class ProjectileExplosionManager {

public:
	NEWDEL_DECL
	ProjectileExplosionManager();
	virtual ~ProjectileExplosionManager();
	
	void OnTick();

	void AddProjectileExplosionNode(ProjectileExplosionNode * pProjectileExplosionNode);
	void RemoveProjectileExplosionNode(ProjectileExplosionNode * pProjectileExplosionNode);
	void RemoveProjectileExplosionNode(int index);

	ProjectileExplosionNode * EnableProjectileExplosionNode();
	void DisableProjectileExplosionNode(int index);


protected:
	int mNumProjectileExplosionNodes;
	int mNumEnabledProjectileExplosions;
	ProjectileExplosionNode * mProjectileExplosionNodes[MAX_NUM_PROJECTILE_EXPLOSIONS];
	
};

#endif //__ProjectileExplosionManager__
