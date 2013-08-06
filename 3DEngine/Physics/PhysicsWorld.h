#ifndef __PhysicsWorld__
#define __PhysicsWorld__


#include "../Config.h"
#include "../PhysicsData.h"
#include "../ModelData.h"
#include "../TL/List.h"

class PhysicsNodeObj
{
public:
	NEWDEL_DECL;
	PhysicsProp * pPhysicsProp;


	void Destroy()
	{
		
	}
};

class PhysicsWorld
{
public:
	NEWDEL_DECL;

	enum PhysicsShapeType
	{
		PHYSICS_BOX = 0,
		PHYSICS_SPHERE = 1,
		PHYSICS_MESH = 2,
	};

	enum PhysicsMoveType
	{
		PHYSICS_MOVE_PHYSICS = 0,
		PHYSICS_MOVE_FIXED_ANIM = 1,
		PHYSICS_MOVE_FIXED = 2,
	};

	PhysicsWorld();
	virtual ~PhysicsWorld();
	virtual void Destroy();

	void Add(PhysicsData * pPhysicsData);
	PhysicsNodeObj * CreatePhysicsNode(PhysicsProp * pPhysicsProp);
	PhysicsNodeObj * GetPhysicsNodeObj(int index);
	void Update();

protected:
	
	
	Map mPhysicsNodes;
};

#endif //__PhysicsWorld__


