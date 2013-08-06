#ifndef __GrenadeNode__
#define __GrenadeNode__

#include "../Config.h"
#include "../TL/List.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#define MAX_NUM_GrenadeS 16

class GrenadeNode {
public:
	NEWDEL_DECL
	enum GrenadeState {
		state_shoot,
		state_fly,
		state_exploding,
		state_finished,

	};

	GrenadeNode();
	virtual ~GrenadeNode();
	
	virtual void OnTick();

	void SetGravity(float gravity);
	void SetSpeed(float speed);
	void SetupTarget();

	inline float GetSpeed()
	{
		return mSpeed;
	}

	void SetShootingRange(float range);

	void SetFriction(float friction);
	void SetHitDistance(float dist);
/*
	inline  float GetHitDistance()
	{
		return mHitDist;
	}
*/

	void SetModelData(ModelData * pModelData );
    
	inline ModelData * GetModelData()
	{
		return mModelData;
	}

	virtual void SetShooterNode(Node * pNode);
	virtual void SetDirectionNode(Node * pNode);
	virtual void SetTargetNode(Node * pNode);

	inline Node * GetShooterNode()
	{
		return mShooterNode;
	}


	inline Node * GetDirectionNode()
	{
		return mDirectionNode;
	}
	void SetTargetPos(PVRTVec3 pos);

	inline const PVRTVec3 & GetTargetPos()
	{
		return mTargetPos;
	}

	virtual void ShootState();
	virtual void FlyState();
	virtual void ExplodingState();
	virtual void FinishedStat();

	inline GrenadeState GetCurrentState()
	{
		return mState;
	}

	inline PVRTVec3 GetGrenadePos()
	{
		return mGrenadePos;
	}

    void PlayExplodingClip();
    
	void Enable();
	void Disable();
	inline bool IsEnabled()
	{
		return mEnabled;
	}

	void SetPower(int power);
	int GetPower();

	ListItem * mListItem;

protected:
	GrenadeState mState;
	Node * mDirectionNode;
	Node * mShooterNode;
	Node * mTargetNode;
	ModelData * mModelData;

	PVRTVec3 mDir;
	PVRTVec3 mGrenadePos;
	PVRTVec3 mGrenadeStartingPos;
	PVRTVec3 mTargetPos;

	float mRange;
	float mSpeed;
	float mFriction;
	float mGravity;
	float mHitDist;
	int mPower;
	bool mEnabled;

	bool mHit;

};

#endif //__GrenadeNode__
