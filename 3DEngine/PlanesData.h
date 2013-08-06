#ifndef __PlanesData__
#define __PlanesData__

#include "Config.h"
#include "Plane.h"
#include "TL/Map.h"
#include "TL/MyString.h"

class PlanesData
{
public:
	NEWDEL_DECL
	PlanesData(void);
	virtual ~PlanesData(void);

	inline int GetNumOfPlanes()
	{
		return mNumOfPlanes;
	}

	inline Plane * GetPlane(int index)
	{
		return &mPlanes[index];
	}

	void SetRadius(float radius);

	inline float GetRadius()
	{
		return mRadius;
	}

	void AllocatePlanes(int numOfPlanes);
	void Transform(PVRTMat4 mtx, PVRTMat4 rotMtx);

	

private:
	int mNumOfPlanes;
	Plane * mPlanes;
	float mRadius;

};

class CollisionMeshData
{
public:
	NEWDEL_DECL
	CollisionMeshData(void);
	virtual ~CollisionMeshData(void);

	inline int GetNumOfCollisionMesh()
	{
		return mNumOfCollisionMesh;
	}

	inline PlanesData * GetPlanesData(int index)
	{
		return &mCollisionMesh[index];
	}

	inline PlanesData * GetPlanesData(String name)
	{
		if (mCollisionMeshTable.FindByKey(name.GetPtr()) == false )
			return NULL;

		int index = (int)mCollisionMeshTable.GetByKey( name.GetPtr() );
		return &mCollisionMesh[index];
	}

	void AllocateCollisionMesh(int numOfCollisionMesh);

public:
	Map mCollisionMeshTable;

private:
	int mNumOfCollisionMesh;
	PlanesData * mCollisionMesh;
};


#endif //__PlanesData__