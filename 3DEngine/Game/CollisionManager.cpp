#include "CollisionManager.h"

NEWDEL_IMPL(CollisionManager)
CollisionManager::CollisionManager()
{
	mNumCollisions = 0;
}

CollisionManager::~CollisionManager()
{
	for (int n=0;n<mNumCollisions;n++)
	{
		delete mCollisions[n];
	}
}

INodesCollision * CollisionManager::CreateCollision(CollisionType type, String senderType, int index)
{
	INodesCollision * pCollision = NULL;
    switch (type) 
    {
        case collision_nodes_to_nodes:
        {
            pCollision = new NodesToNodesCollision(senderType);
            break;
        }
        case collision_single_node_to_nodes:
        {
            pCollision = new SingleNodeToNodesCollision(senderType);
            break;
        }
        case collision_bullets_to_nodes:
        {
            pCollision = new BulletsToNodesCollision(senderType);
            break;
        }
        case collision_grenades_to_nodes:
        {
            pCollision = new GrenadesToNodesCollision(senderType);
            break;
        }
        case collision_bullets_to_single_node:
        {
            pCollision = new BulletsToSingleNodeCollision(senderType);
            break;
        }
        case collision_nodes_to_screen:
        {
            pCollision = new SingleNodeToScreenCollision(senderType);
            break;
        }
        default:
            STOP;
    }

	mCollisions[index] = pCollision;
	mNumCollisions++;

	return pCollision;
}



void CollisionManager::OnTick()
{	
	for (int n=0;n<mNumCollisions;n++)
	{
		mCollisions[n]->OnTick();
	}

}


