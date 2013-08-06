#ifndef __BombNode__
#define __BombNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"
#include "../LocatorData.h"
#include "../Events/EventSender.h"
#include "../Events/EventManager.h"
#include "../Events/EventReceiver.h"

#define MAX_NUM_BOMBS 32

class BombNode {
public:
	NEWDEL_DECL
	enum BombState {
		state_idle,
		state_moving,
		state_exploding,
		state_finished,

	};

	BombNode();
	virtual ~BombNode();
	
	virtual void OnTick();
	
	void SetHitDistance(float dist);
	inline  float GetHitDistance()
	{
		return mHitDist;
	}


	void SetModelData(ModelData * pModelData);
	inline ModelData * GetModelData()
	{
		return mModelData;
	}

	virtual void SetLocator(Locator * pLocator);
	virtual void SetShooterNode(Node * pNode);
	virtual void SetTargetNode(Node * pNode);

	inline Node * GetShooterNode()
	{
		return mShooterNode;
	}

	void SetTargetPos(PVRTVec3 pos);

	inline const PVRTVec3 & GetTargetPos()
	{
		return mTargetPos;
	}

	void SetBombHitFlag();

	virtual void IdleState();
	virtual void MovingState();
	virtual void ExplodingState();
	virtual void FinishedStat();

	BombState GetCurrentState()
	{
		return mState;
	}

	void SwitchToState(BombState state)
	{
		mState = state;
	}

	void SetBombPos(PVRTVec3 pos);

	inline PVRTVec3 GetBombPos()
	{
		return mBombPos;
	}

	void Enable();
	void Disable();
	inline bool IsEnabled()
	{
		return mEnabled;
	}

	void SetPower(int power);
	int GetPower();

	void SetAttachNode(Node * pNode);
	inline Node * GetAttachNode()
	{
		return mAttachNode;
	}

	void SetAutoDistancePick(bool flag)
	{
		mAutoDistancePick = flag;
	}

	void SetAutoDistanceExplode(bool flag)
	{
		mAutoDistanceExplode = flag;
	}

	void SetAutoTimerExplode(bool flag)
	{
		mAutoTimerExplode = flag;
	}

	bool GetAutoDistancePick()
	{
		return mAutoDistancePick;
	}

	bool GetAutoDistanceExplode()
	{
		return mAutoDistanceExplode;
	}

	bool GetAutoTimerExplode()
	{
		return mAutoTimerExplode;
	}

protected:
	BombState mState;
	Node * mShooterNode;
	Node * mTargetNode;
	Node * mAttachNode;

	ModelData * mModelData;
	Locator * mLocator;
	
	PVRTVec3 mBombPos;
	PVRTVec3 mTargetPos;

	float mHitDist;

	int mPower;
	bool mEnabled;

	bool mAutoDistancePick;
	bool mAutoDistanceExplode;
	bool mAutoTimerExplode;

	bool mHit;

};

#endif //__BombNode__
