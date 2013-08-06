#ifndef __GraphMoveToTarget__
#define __GraphMoveToTarget__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "GraphDataUser.h"

class RefSceneData;

class GraphMoveToTarget : public GraphDataUser {

public:
	enum MoveMode {
		TRANSLATE,
		ROTATE,
		TRANSLATE_ROTATE
	};

	NEWDEL_DECL;
	GraphMoveToTarget();
	virtual ~GraphMoveToTarget();


	void InitQuads();

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

	void SetNode(Node * pNode);

	inline Node * GetNode()
	{
		return mNode;
	}

	inline bool IsAtTarget()
	{
		return mAtTarget;
	}

	inline void SetMoveMode(MoveMode mode)
	{
		mMoveMode = mode;
	}

	inline MoveMode GetMoveMode()
	{
		return mMoveMode;
	}

	void SetMinDistance(float dist);
	float GetMinDistance();

	void SetMaxDistance(float dist);
	float GetMaxDistance();

	void SetWalkDistance(float dist);
	float GetWalkDistance();

	void SetFront(float front);
	float GetFront();

	void SetupDirection();

	void SetRefreshRate(int rate)
	{
		mRefreshRate = rate;
	}

	GraphNode * FindRandomGraphNode();
	void SetCurrentGraphNodeRandom();
	void SearchNextGraphNode();


	inline GraphNode * GetCurrentGraphNode()
	{
		return mCurrentGraphNode;
	}

	inline GraphNode * GetNextGraphNode()
	{
		return mNextGraphNode;
	}

	virtual void OnTick();

protected:
	float mWalkDistance;
	float mMinDist;
	float mMaxDist;
	float mFront;
	bool mAtTarget;

	Node * mTargetNode;
	Node * mNode;
	PVRTVec3 mTargetPos;

	RefSceneData * mRefSceneData;
	GraphNode * mTargetGraphNode;
	GraphNode * mCurrentGraphNode;
	GraphNode * mLastGraphNode;
	GraphNode * mNextGraphNode;

	PVRTVec3 mNextPos;
	PVRTVec3 mCurrentPos;
	PVRTVec3 mDirection;
	PVRTQUATERNION mRotQuat;

	int mRefreshRate;
	int mTickCount;
	float mLastDist;

	MoveMode mMoveMode;

};

#endif //__GraphMoveToTarget__
