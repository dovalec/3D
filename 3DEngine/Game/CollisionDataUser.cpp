#include "CollisionDataUser.h"
#include "../PlanesData.h"
#include "../AI/Collision.h"

NEWDEL_IMPL(CollisionDataUser);

bool CollisionDataUser::CheckCollision( Node * pNode , float walkDistance, bool stopOnFirstCollision)
{
    
    JoyStick * pJoyStick = JoyStick::GetJoyStick();
    
	PVRTVec3 nodeWorldPos = pNode->GetTranslation();
	PVRTQUATERNION nodeRot = pNode->GetRotation();
	
	PVRTVec2 vecLeft = pJoyStick->GetVectorLeft();
	PVRTVec2 vecRight = pJoyStick->GetVectorRight();

	vecLeft.normalize();
	vecLeft.y*=-1.0f;

	PVRTVec2 nodeNewPos(nodeWorldPos.x, nodeWorldPos.z);
	PVRTVec2 globalTan(0.0f,0.0f);

	int planesCollisionCount = 0;


	for (int quadDataCount=0; quadDataCount<mNumQuadsData; quadDataCount++)
	{
		QuadTree * pQuadTree = mQuadsData[quadDataCount];
		PVRTVec2 pos(nodeWorldPos.x, nodeWorldPos.z);
		static QuadNode * quadNodes[256]={0}; 
		int quadNodeCount = 0;

		pQuadTree->GetQuads(nodeWorldPos.x, nodeWorldPos.z, quadNodes, quadNodeCount);
		quadNodeCount--;

		bool stopCollisionCheck = false;

		for (int quad = quadNodeCount ; quad >=0 ; quad--)
		{
			QuadNode * pQuadNode = quadNodes[quad];

			List & dataList = pQuadNode->GetDataList();
			ListIterator listIter(dataList);
			while( PlanesData * pPlanesData = (PlanesData*)listIter.GetPtr())				
			{			
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
						planesCollisionCount++;						
					}

				}
				if (planesCollisionCount > 0 && stopOnFirstCollision)
				{
					stopCollisionCheck = true;
					break;
				}
			}

			if (stopCollisionCheck)
				break;
		}

		if (stopCollisionCheck)
			break;
	}


	PVRTVec2 walkDir = nodeNewPos - mLastPlayerPos;
	float walkDelta = walkDir.length();
	mLastPlayerPos = nodeNewPos;


	if (planesCollisionCount > 0)
	{
		globalTan/=(float)planesCollisionCount;
		float deltaAngle;
		if (!mCollided)
		{
			deltaAngle = acos(walkDir.dot(globalTan));
			if (  deltaAngle >= PVRT_PI*0.5f )
				mCollided = true;
			else
				mCollided = false;
		}
		else 
		{	
					
			PVRTVec4 vecRot( vecLeft.x , 0, vecLeft.y, 1);
			PVRTMat4 nodeRotMtx;
			PVRTMatrixRotationQuaternion(nodeRotMtx, nodeRot);
			vecRot = nodeRotMtx * vecRot;
			PVRTVec2 joyRoy(vecRot.x, vecRot.z);
	
			//PVRTVECTOR3 Y_AXIS={0,1,0};
			//float nodeWorldAngle = 0.0f;
			//PVRTMatrixQuaternionToAxisAngle(nodeRot, Y_AXIS, nodeWorldAngle);
			//deltaAngle = abs(nodeWorldAngle - globalTan); //acos(vecLeft.dot(globalTan));
			deltaAngle = acos( joyRoy.dot(globalTan) );

			if ( deltaAngle <= PVRT_PI*0.5f )
				mCollided = true;
			else
				mCollided = false;
		}

		//static char txt[255]={0};
		//sprintf(txt,"%f \n",deltaAngle);
		//OutputDebugStr(txt);
	}

	return mCollided;
}