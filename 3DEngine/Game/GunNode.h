#ifndef __GunNode__
#define __GunNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../LocatorData.h"
#include "../ModelData.h"
#include "PlayerNode.h"
#include "BulletManager.h"

class GunNode {
public:
	NEWDEL_DECL;
	
	enum GunState {
		state_init,
		state_idle,
		state_shoot,
		state_exploded,
		state_finished,
	};

	GunNode();
	virtual ~GunNode();
	
	virtual void OnTick();

	inline GunState GetCurrentState()
	{
		return mState;
	}

	virtual void HandleBullet();

	virtual void ExplodeGun();

	virtual void SetModelData(ModelData * pModelData);
	virtual void SetPlayerNode(PlayerNode * pPlayerNode);
	virtual void SetLocator(Locator * pLocator);

	virtual void InitState() = 0;
	virtual void ShootState() = 0;
	virtual void IdleState() = 0;
	virtual void ExplodedState() = 0;
	virtual void FinishedState() = 0;

	virtual void Fire() = 0;

	inline BulletManager * GetBulletManager()
	{
		return mBulletManager;
	}

	void SetShootingRate(int rate);
	inline int GetShootingRate(int rate)
	{
		return mShootingRate;
	}

	inline BulletManager * GetGunNode()
	{
		return mBulletManager;
	}

protected:
	
	PlayerNode * mPlayerNode;
	ModelData * mModelData;
	Locator * mLocator;
	BulletManager * mBulletManager;
	int mShootingRate;
	int mShootCount;
	int mUnderAttackCount;
	GunState mState;
};

#endif //__GunNode__
