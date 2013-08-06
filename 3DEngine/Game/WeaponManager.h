#ifndef __WeaponManager__
#define __WeaponManager__

#include "../Config.h"
#include "../GlobalHeap.h"
#include "../ModelData.h"
#include "../OverlayFace.h"
#include "SoldierWeapon.h"

#include "../TL/Map.h"



#define MAX_PLAYER_GUNS 128

class WeaponManager {
public:
	NEWDEL_DECL;

	WeaponManager();
	virtual ~WeaponManager();


	inline SoldierWeapon * GetCurrentWeapon()
	{
		return mCurrentWeapon;
	}

	inline BulletManager * GetBulletManager()
	{
		return &mBulletManager;
	}


	void LoadWeapon(const char * name, int shootingNodeID );
	void LoadWeapon(const char * name, int shootingNodeID , int cartridgeSize);
	void LoadWeapon(const char * name, int shootingNodeID , WeaponParams params);
	void Init(int numOfBullets);
	void InitBullets(int numOfBullets);

	SoldierWeapon * SwitchWeapon();

	void OnTick();
	BulletNode * Fire(bool noisy);


protected:
	
	BulletManager mBulletManager;

	String mWeaponsName[MAX_PLAYER_GUNS];
	
	Map mWeaponsMap;

	int mCurrentWeaponIndex;
	int mWeaponCount;
	SoldierWeapon * mCurrentWeapon;

	int mShootingRate;
	int mShootCount;
	
};

#endif //__WeaponManager__
