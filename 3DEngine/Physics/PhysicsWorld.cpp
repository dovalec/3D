#include "PhysicsWorld.h"

NEWDEL_IMPL(PhysicsWorld)
NEWDEL_IMPL(PhysicsNodeObj)

PhysicsWorld::PhysicsWorld() 
{
	
		
}

PhysicsWorld::~PhysicsWorld() 
{
	Destroy();
}

void PhysicsWorld::Destroy() 
{
	
	MapIterator iter(mPhysicsNodes);
	while (PhysicsNodeObj * pNode = (PhysicsNodeObj*)iter.GetPtr())
	{
		pNode->Destroy();
	}
}

PhysicsNodeObj * PhysicsWorld::GetPhysicsNodeObj(int index)
{
	PhysicsNodeObj * pRet = (PhysicsNodeObj*)mPhysicsNodes.GetByKey(index);
	return pRet;
}

void PhysicsWorld::Add( PhysicsData * pPhysicsData )
{
	int numOfPhysics = pPhysicsData->GetNumOfPhysics();
	for (int n=0;n<numOfPhysics;n++)
	{
		PhysicsProp * pPhysicsProp = pPhysicsData->GetPhysicsProp(n);
		PhysicsNodeObj * pPhysicsNodeObj = CreatePhysicsNode(pPhysicsProp);
	}
}


PhysicsNodeObj * PhysicsWorld::CreatePhysicsNode(PhysicsProp * pPhysicsProp)
{
	PhysicsNodeObj * pPhysicsNodeObj = new PhysicsNodeObj();
	
	pPhysicsNodeObj->pPhysicsProp = pPhysicsProp;
	
	mPhysicsNodes.Add(pPhysicsProp->index, pPhysicsNodeObj);

	PhysicsShapeType type = (PhysicsShapeType)pPhysicsProp->shape;
	PhysicsMoveType move = (PhysicsMoveType)pPhysicsProp->move;

	Node * pNode = pPhysicsProp->node;
	PVRTVec3 worldPos = pNode->GetWorldTranslation();
	PVRTQUATERNION  quat = pNode->GetRotation();
	PVRTVec3 scale = pNode->GetScale();
		
	if (type == PHYSICS_BOX)
	{
		
	}
	else if (type == PHYSICS_SPHERE)
	{
		
    }


	if (move == PHYSICS_MOVE_PHYSICS)
	{
		
	}



	if (move == PHYSICS_MOVE_FIXED_ANIM)
	{
		/*
			If you plan to animate or move static objects, you should flag them as kinematic.
			Also disable the sleeping/deactivation for them during the animation.
			This means Bullet dynamics world will get the new worldtransform from the btMotionState every simulation frame.
		*/
	}

		return pPhysicsNodeObj;
}

void PhysicsWorld::Update() 
{
	
	

}
