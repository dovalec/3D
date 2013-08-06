#include "StopCircleUser.h"
NEWDEL_IMPL(StopCircleUser);

bool StopCircleUser::CheckCollision( PVRTVec3 nodeWorldPos)
{
    JoyStick * pJoyStick = JoyStick::GetJoyStick();
    
	PVRTVec2 vecLeft = pJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = pJoyStick->GetVectorRight();

	vecLeft.normalize();
	vecLeft.y*=-1.0f;


	PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);
	PVRTVec2 globalDir(0.0f, 0.0f);
	
	int collisionCount = 0;

	for (int c = 0 ; c < mNumStopCircles; c++)
	{
		StopCircle * pCircle = mStopCircles[c];
		PVRTVec2 circlePos(pCircle->x, pCircle->z);
		PVRTVec2 dirVec = nodeNewPos - circlePos;
		float dist = dirVec.lenSqr();
		if (dist < MM(pCircle->radius))
		{

			globalDir+=dirVec.normalized();
			collisionCount++;
		}

	}

	if (collisionCount)
	{
		globalDir/=(float)collisionCount;
		globalDir.normalize();

		float deltaAngle = acos(vecLeft.dot(globalDir));		
		if (  deltaAngle >= PVRT_PI*0.5f )
		{
			return true;
		}
	
	}

	return false;
}