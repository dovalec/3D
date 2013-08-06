#ifndef __ExplosionNode__
#define __ExplosionNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#define MAX_NUM_EXPLOSIONS 32

class ExplosionNode {
public:
	NEWDEL_DECL
	enum ExplosionState {
		state_explode,
		state_exploding,
		state_hit,
		state_missed,
		state_finished,

	};

	ExplosionNode();
	virtual ~ExplosionNode();
	
	virtual void OnTick();
	
	void SetSpeed(float speed);
	void SetRange(float range);

	void SetFriction(float friction);
	void SetHitDistance(float dist);

	void SetTextureRows(int rows);
	void SetTextureCols(int cols);
	void SetSubImageCount(int count);

	void SetMesh(Mesh * pMesh)
	{
		mMesh = pMesh;
	}

	inline Mesh * GetMesh()
	{
		return mMesh;
	}

	virtual void SetModelData(ModelData * pModelData);
	virtual void SetShooterNode(Node * pNode);
	virtual void SetTargetNode(Node * pNode);

	virtual void ExplodeState();
	virtual void ExplodingState();
	virtual void HitState();
	virtual void MissedStat();


	inline ExplosionState GetCurrentState()
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
	ExplosionState mState;
	Node * mShooterNode;
	Node * mTargetNode;
	ModelData * mModelData;
	PVRTVec3 mDir;
	PVRTVec3 mExplosionPos;
	PVRTVec3 mExplosionStartingPos;
	float mRange;
	float mSpeed;
	float mFriction;
	float mGravity;
	float mHitDist;
	int mPower;
	bool mEnabled;

	int mTextureRows;
	int mTextureCols;
	int mSubImageCount;
	int mCurrentRow;
	int mCurrentCol;

	Mesh * mMesh;


};

#endif //__ExplosionNode__
