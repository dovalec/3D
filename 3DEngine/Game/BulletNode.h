#ifndef __BulletNode__
#define __BulletNode__

#include "../Config.h"
#include "../TL/List.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#define MAX_NUM_BULLETS 16

class BulletNode {
public:
	NEWDEL_DECL
	enum BulletState {
		state_shoot,
		state_fly,
		state_hit,
		state_missed,
		state_finished,

	};

	BulletNode();
	virtual ~BulletNode();
	
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

	void SetModelData(ModelData * pModelData);
	inline ModelData * GetModelData()
	{
		return mModelData;
	}
    
    inline void SetNoisy(bool noisy)
    {
        mNoisy = noisy;
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

	void SetBulletHitFlag();

	virtual void ShootState();
	virtual void FlyState();
	virtual void HitState();
	virtual void MissedStat();
	virtual void FinishedStat();

	inline BulletState GetCurrentState()
	{
		return mState;
	}

	inline PVRTVec3 GetBulletPos()
	{
		return mBulletPos;
	}

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
	BulletState mState;
	Node * mDirectionNode;
	Node * mShooterNode;
	Node * mTargetNode;
	ModelData * mModelData;
	PVRTVec3 mDir;
	PVRTVec3 mBulletPos;
	PVRTVec3 mBulletStartingPos;
	PVRTVec3 mTargetPos;

	float mRange;
	float mSpeed;
	float mFriction;
	float mGravity;
	float mHitDist;
	int mPower;
	bool mEnabled;

	bool mHit;
    bool mNoisy;

};

#endif //__BulletNode__
