#include "PhysicsData.h"
#include "ModelData.h"
#include "Node.h"

NEWDEL_IMPL(PhysicsData)
NEWDEL_IMPL(PhysicsProp)


PhysicsData::PhysicsData(void)
{
	
}

PhysicsData::~PhysicsData(void)
{
	delete [] mPhysicsProps;
}

void PhysicsData::AllocatePhysics(int numOfPhysics)
{
	mPhysicsProps = new PhysicsProp[numOfPhysics];
	mNumOfPhysics = numOfPhysics;
}
