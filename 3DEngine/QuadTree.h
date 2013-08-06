#ifndef __QuadTree__
#define __QuadTree__

#include "Config.h"
#include "TL/List.h"

class Node;

class QuadNode
{
public:
	NEWDEL_DECL
	QuadNode()
	{
	}
	virtual ~QuadNode()
	{
		delete [] mQuadNodes;
	}

	void AllocateChildren(int depth, int & count);
	void GetQuadsCameraFrustum(QuadNode ** retQuadNodes, int & retQuadNodeCount, PVRTMat4 * pLookMtx);
	void GetQuadsTriangleFrustum(PVRTVec2 A, PVRTVec2 B, PVRTVec2 C, QuadNode ** retQuadNodes, int & retQuadNodeCount);
	void GetQuadsForLine(PVRTVec2 A, PVRTVec2 MID, QuadNode ** retQuadNodes, int & retQuadNodeCount);
	void GetQuads(float x, float z, float radius, QuadNode ** retQuadNodes, int & retQuadNodeCount);
	void GetQuads(float x, float z, QuadNode ** retQuadNodes, int & retQuadNodeCount);
	bool Add(void * pData, float x, float z, float radius);


	inline void SetCenter(PVRTVec2 center)
	{
		mCenter = center;
	}

	inline void SetCenter(float x, float y)
	{
		mCenter.x = x;
		mCenter.y = y;
	}
	
	inline void SetRadius(float radius)
	{
		mRadius = radius;
	}

	inline PVRTVec2 GetCenter()
	{
		return mCenter;
	}
	
	inline float GetRadius()
	{
		return mRadius;
	}

	inline List & GetDataList()
	{
		return mDataList;
	}

protected:
	QuadNode * mQuadNodes;
	PVRTVec2 mCenter;
	float mRadius;
	List mDataList;

};

class QuadTree : public QuadNode
{
public:
	NEWDEL_DECL
	QuadTree(void)
	{

	}

	virtual ~QuadTree(void)
	{
		
	}

	
	void Allocate(int depth, float radius)
	{
		mRadius	= radius;
		mCenter *= 0.0f;
		int count = 0;
		QuadNode::AllocateChildren(depth, count);
	}

};


#endif //__QuadTree__