#ifndef __CollisionManager__
#define __CollisionManager__

#include "../Config.h"

#include "NodesToNodesCollision.h"
#include "BulletsToSingleNodeCollision.h"
#include "SingleNodeToNodesCollision.h"
#include "BulletsToNodesCollision.h"
#include "GrenadesToNodesCollision.h"
#include "SingleNodeToScreenCollision.h"

class CollisionManager {

public:
	NEWDEL_DECL;

	enum CollisionType {
		collision_nodes_to_nodes,
		collision_single_node_to_nodes,
		collision_bullets_to_nodes,
        collision_grenades_to_nodes,
		collision_bullets_to_single_node,
		collision_nodes_to_screen,
	};

	CollisionManager();
	virtual ~CollisionManager();
	
	void OnTick();

	inline INodesCollision * GetCollision(int index)
	{
		return mCollisions[index];
	}

	INodesCollision * CreateCollision(CollisionType type, String senderType, int index);

protected:
	int mNumCollisions;
	INodesCollision * mCollisions[MAX_COLLISION];
	
};

#endif //__CollisionManager__
