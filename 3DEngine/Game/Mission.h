#ifndef __BulletNode__
#define __BulletNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#define MAX_NUM_BULLETS 64

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
	
	virtual void Update();
	virtual void Apply();

	
	void SetSpeed(float speed);
	void SetFriction(float friction);
	void SetHitDistance(float dist);
	virtual void SetModelData(ModelData * pModelData);
	virtual void SetShooterNode(Node * pNode);
	virtual void SetTargetNode(Node * pNode);

	virtual void ShootState();
	virtual void FlyState();
	virtual void HitState();
	virtual void MissedStat();

	BulletState GetCurrentState()
	{
		return mState;
	}

	void Enable();
	void Disable();
	inline bool IsEnabled()
	{
		return mEnabled;
	}

	void SetPower(int power);
	int GetPower();

protected:
	BulletState mState;
	Node * mShooterNode;
	Node * mTargetNode;
	ModelData * mModelData;
	PVRTVec3 mDir;
	PVRTVec3 mBulletPos;
	float mSpeed;
	float mFriction;
	float mGravity;
	float mHitDist;
	int mPower;
	bool mEnabled;

};

#endif //__BulletNode__
