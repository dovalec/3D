#include "PlanesDataUser.h"
#include "../AI/Collision.h"

NEWDEL_IMPL(PlanesDataUser);

bool PlanesDataUser::CheckCollision(PVRTVec3 nodeWorldPos, float walkDistance)
{
     JoyStick * pJoyStick = JoyStick::GetJoyStick();
    
	PVRTVec2 vecLeft = pJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = pJoyStick->GetVectorRight();

	vecLeft.normalize();
	vecLeft.y*=-1.0f;

	PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);
	PVRTVec2 globalTan(0.0f,0.0f);

	int collisionCount = 0;

	for (int p=0;p<mNumPlanesData;p++)
	{
		PlanesData * pPlanesData = mPlanesData[p];
		int numOfPlanes = pPlanesData->GetNumOfPlanes();
		for (int n=0;n<numOfPlanes;n++)
		{
			Plane * plane = pPlanesData->GetPlane(n);
			Line2D line;
			line.p0.x = plane->x1;
			line.p0.y = plane->z1;

			line.p1.x = plane->x2;
			line.p1.y = plane->z2;
			
			float dist = Collision::Vec2DistanceFromLine2D(line, nodeNewPos);
			if (dist < MM(walkDistance))
			{								
				PVRTVec2 wall(plane->nx, plane->nz);
				globalTan+=wall.normalized();
				collisionCount++;
			}
												
		}
	}

	if (collisionCount > 0)
	{
		globalTan/=(float)collisionCount;

		float deltaAngle = acos(vecLeft.dot(globalTan));

		//static char txt[255]={0};
		//sprintf(txt,"%f \n",deltaAngle);
		//OutputDebugStr(txt);
		
		if (  deltaAngle >= PVRT_PI*0.5f )
		{
			return true;
		}
	}

	return false;
}