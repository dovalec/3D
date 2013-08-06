#ifndef __MoveToTarget__
#define __MoveToTarget__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "StopWireUser.h"

class MoveToTarget {//: public StopWireUser {
public:
	NEWDEL_DECL
	MoveToTarget();
	virtual ~MoveToTarget();


	void SetTargetNode(Node * pNode);

	inline Node * GetTargetNode()
	{
		return mTargetNode;
	}

	void SetTargetPos(PVRTVec3 pos);

	inline const PVRTVec3 & GetTargetPos()
	{
		return mTargetPos;
	}


	void SetLookAtTargetNode(Node * pNode);

	inline Node * GetLookAtTargetNode()
	{
		return mLookAtTargetNode;
	}

	void SetLookAtTargetPos(PVRTVec3 pos);

	inline const PVRTVec3 & GetLookAtTargetPos()
	{
		return mLookAtTargetPos;
	}

	void SetNode(Node * pNode);

	inline Node * GetNode()
	{
		return mNode;
	}

	inline bool IsAtTarget()
	{
		return mAtTarget;
	}

	void SetMinDistance(float dist);
	float GetMinDistance();

	void SetMaxDistance(float dist);
	float GetMaxDistance();

	void SetWalkDistance(float dist);
	float GetWalkDistance();

	void SetFront(float front);
	float GetFront();

	virtual void CalcNodePos();

protected:
	float mWalkDistance;
	float mMinDist;
	float mMaxDist;
	float mFront;
	bool mAtTarget;

	Node * mTargetNode;
	Node * mLookAtTargetNode;
	Node * mNode;
	PVRTVec3 mTargetPos;
	PVRTVec3 mLookAtTargetPos;

};

#endif //__MoveToTarget__
