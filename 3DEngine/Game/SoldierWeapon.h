#ifndef __SoldierWeapon__
#define __SoldierWeapon__

#include "../Config.h"
#include "../ModelData.h"
#include "BulletManager.h"

class WeaponParams
{
	NEWDEL_DECL;
public:
	int mShootHot;
	int mColdingTime;
	int mCartridgeSize;
	int mCartridgeCount;
	int mCartridgeBulletsLeft;
};

class SoldierWeapon {

public:
	NEWDEL_DECL
	SoldierWeapon();
	virtual ~SoldierWeapon();

	void OnTick();

	void SetModelData(ModelData * pModelData)
	{
		mModelData = pModelData;
	}

	inline ModelData * GetModelData()
	{
		return mModelData;
	}


	void SetUITexture(PVRTexture * pTexture)
	{
		mUITexture = pTexture;
	}

	
	inline PVRTexture * GetUITexture()
	{
		return mUITexture;
	}


	
	void SetShootingNode(Node * pNode)
	{
		mShootingNode = pNode;
	}

	inline Node * GetShootingNode()
	{
		return mShootingNode;
	}

	inline WeaponParams & GetWeaponParams()
	{
		return mParams;
	}

	void SetWeaponParams(WeaponParams params)
	{	
		mParams = params;
	}

protected:
	WeaponParams mParams;
	ModelData * mModelData;
	PVRTexture * mUITexture;
	Node * mShootingNode;

};

#endif //__SoldierWeapon__
