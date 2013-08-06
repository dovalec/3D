#include "DoorsDataUser.h"
#include "../AI/Collision.h"

NEWDEL_IMPL(DoorsDataUser);

bool DoorsDataUser::CheckCollision(PVRTVec3 nodeWorldPos, float walkDistance)
{
    JoyStick * pJoyStick = JoyStick::GetJoyStick();
    
	PVRTVec2 vecLeft = pJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = pJoyStick->GetVectorRight();

	vecLeft.normalize();
	vecLeft.y*=-1.0f;

	PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);
	PVRTVec2 globalTan(0.0f,0.0f);

	int collisionCount = 0;

	for (int p=0;p<mNumDoorsData;p++)
	{
		DoorsData * pDoorsData = mDoorsData[p];
		int numOfDoors = pDoorsData->GetNumOfDoors();
		for (int d=0;d<numOfDoors;d++)
		{
			Door * pDoor = pDoorsData->GetDoor(d);
			if (pDoor->GetState() == Door::door_state_open)
				continue;

			int numOfDoorEdges = pDoor->GetNumOfDoorEdges();
			for (int e = 0 ; e< numOfDoorEdges ; e++)
			{
				DoorEdge * edge = pDoor->GetDoorEdge(e);
				Line2D line;
				line.p0.x = edge->x1;
				line.p0.y = edge->z1;

				line.p1.x = edge->x2;
				line.p1.y = edge->z2;
				
				float dist = Collision::Vec2DistanceFromLine2D(line, nodeNewPos);
				if (dist < MM(walkDistance))
				{								
					PVRTVec2 wall(edge->nx, edge->nz);
					globalTan+=wall.normalized();
					collisionCount++;
				}
			}
												
		}
	}

	if (collisionCount > 0)
	{
		globalTan/=(float)collisionCount;

		float deltaAngle = acos(vecLeft.dot(globalTan));
		if (  deltaAngle >= PVRT_PI*0.5f )
		{
			return true;
		}
	}

	return false;
}