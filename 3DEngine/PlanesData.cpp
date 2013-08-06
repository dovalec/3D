#include "PlanesData.h"
#include "ModelData.h"

NEWDEL_IMPL(PlanesData)
NEWDEL_IMPL(CollisionMeshData)


PlanesData::PlanesData(void)
{
	mPlanes = NULL;
	mRadius = 0.0f;
}

PlanesData::~PlanesData(void)
{
	delete [] mPlanes;
}

void PlanesData::AllocatePlanes(int numOfPlanes)
{
	mPlanes = new Plane[numOfPlanes];
	mNumOfPlanes = numOfPlanes;
}


void PlanesData::SetRadius(float radius)
{
	mRadius = radius;
}

void PlanesData::Transform(PVRTMat4 mtx, PVRTMat4 rotMtx)
{
	for (int n=0;n<mNumOfPlanes;n++)
	{
		Plane & plane = mPlanes[n];
		PVRTVec4 vec1( plane.x1, 0,  plane.z1, 1);
		PVRTVec4 vec2( plane.x2, 0,  plane.z2, 1);
		PVRTVec4 vecn( plane.nx, plane.ny,  plane.nz, 1);

		vec1 = mtx * vec1;
		vec2 = mtx * vec2;
		if (vec1 == vec2)
		{
			STOP;
		}
		vecn = rotMtx * vecn;

		vecn.w = 0;
		vecn.normalize();

		plane.x1 = vec1.x;
		plane.z1 = vec1.z;

		plane.x2 = vec2.x;
		plane.z2 = vec2.z;

		plane.nx = vecn.x;
		plane.ny = vecn.y;
		plane.nz = vecn.z;

	}
}


/////////////////////////////////////////////////////////////

CollisionMeshData::CollisionMeshData()
{
	mCollisionMesh = NULL;
}

CollisionMeshData::~CollisionMeshData()
{
	delete [] mCollisionMesh;
}


void CollisionMeshData::AllocateCollisionMesh(int numOfCollisionMesh)
{
	mCollisionMesh = new PlanesData[numOfCollisionMesh];
	mNumOfCollisionMesh = numOfCollisionMesh;
}
