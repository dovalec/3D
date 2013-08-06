#ifndef __PhysicsData__
#define __PhysicsData__

#include "Config.h"
#include "TL/Map.h"

class Node;

class PhysicsProp
{
public:
	NEWDEL_DECL;
	Node * node;
	int index;
	int move;
	int shape;
	float deactivationTime;
	float friction;
	float restitution;
	float mass;
	
	float radius;
	float width;
	float height;
	float depth;
};

class PhysicsData
{
public:
	NEWDEL_DECL
	PhysicsData(void);
	virtual ~PhysicsData(void);

	inline int GetNumOfPhysics()
	{
		return mNumOfPhysics;
	}

	inline PhysicsProp * GetPhysicsProp(int index)
	{
		return &mPhysicsProps[index];
	}

	void AllocatePhysics(int numOfPhysics);

	int mNumOfPhysics;
	PhysicsProp * mPhysicsProps;
};


#endif //__PhysicsData__