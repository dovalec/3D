#include "WeaponManager.h"
#include "../ResourceLoader.h"

NEWDEL_IMPL(WeaponManager)
WeaponManager::WeaponManager()
{
	mCurrentWeaponIndex = 0;
	mWeaponCount = 0;
	mCurrentWeapon = NULL;

	mShootingRate = 3;
	mShootCount = 0;
}

WeaponManager::~WeaponManager()
{
	
}

void WeaponManager::OnTick()
{
	mBulletManager.OnTick();
}


void WeaponManager::LoadWeapon(const char * name, int shootingNodeID )
{
	WeaponParams params;
	params.mShootHot = 100;
	params.mColdingTime = 20;

	params.mCartridgeSize = 30;
	params.mCartridgeCount = 8;
	params.mCartridgeBulletsLeft = params.mCartridgeSize;
	LoadWeapon(name, shootingNodeID, params);
}

void WeaponManager::LoadWeapon(const char * name, int shootingNodeID , int cartridgeSize)
{
	WeaponParams params;
	params.mShootHot = 100;
	params.mColdingTime = 20;

	params.mCartridgeSize = cartridgeSize;
	params.mCartridgeCount = 8;
	params.mCartridgeBulletsLeft = params.mCartridgeSize;

	LoadWeapon(name, shootingNodeID, params);
}

void WeaponManager::LoadWeapon(const char * name, int shootingNodeID , WeaponParams params)
{

	ResourceLoader & resaourceLoader = ResourceLoader::GetResourceLoader();

	SoldierWeapon * pSoldierWeapon = new SoldierWeapon();	
	pSoldierWeapon->SetWeaponParams(params );
	
	static char modName[128]={0};
	sprintf(modName, "%s.mod",name);


	//ModelData * pWeaponModel = resaourceLoader.LoadModel( "M16.mod" );
	ModelData * pWeaponModel = resaourceLoader.LoadModel( modName , false, false );

	pWeaponModel->GetRoot()->SetVisible(false);
	pWeaponModel->GetRoot()->SetUseOcclusionCulling(false);
	//pWeaponModel->GetRoot()->SetScale(0.25);

	pSoldierWeapon->SetModelData( pWeaponModel ); 
	pSoldierWeapon->SetShootingNode( (Node*)pWeaponModel->mTransforms.GetArray( shootingNodeID ) );

    /* Remarked due to DC1 weapon ...
     
	static char uiImageName[128]={0};
	sprintf(uiImageName, "BLK_%s.pvr",name);

	PVRTexture * pUITexture = resaourceLoader.LoadTexture( uiImageName , true);
	pSoldierWeapon->SetUITexture ( pUITexture );
    */
    
	mWeaponsMap.Add(name, pSoldierWeapon);

	mWeaponsName[mWeaponCount] = name;
	mWeaponCount++;
}

void WeaponManager::Init(int numOfBullets)
{
	const char * weaponName = mWeaponsName[mCurrentWeaponIndex].GetPtr();
	SoldierWeapon * pSoldierWeapon = (SoldierWeapon*)mWeaponsMap.GetByKey(weaponName);
	
	mCurrentWeapon = pSoldierWeapon;
	mCurrentWeapon->GetModelData()->GetRoot()->SetVisible(true);

	InitBullets(numOfBullets);
}

SoldierWeapon * WeaponManager::SwitchWeapon()
{
	mCurrentWeaponIndex++;
	if (mCurrentWeaponIndex == mWeaponCount)
		mCurrentWeaponIndex = 0;

	const char * weaponName = mWeaponsName[mCurrentWeaponIndex].GetPtr();
	SoldierWeapon * pSoldierWeapon = (SoldierWeapon*)mWeaponsMap.GetByKey(weaponName);
	
	if (mCurrentWeapon)
	{
		mCurrentWeapon->GetModelData()->GetRoot()->SetVisible(false);
	}

	mCurrentWeapon = pSoldierWeapon;
	mCurrentWeapon->GetModelData()->GetRoot()->SetVisible(true);

	return mCurrentWeapon;
}


void WeaponManager::InitBullets(int numOfBullets)
{
	
	ResourceLoader & resourceLoader = ResourceLoader::GetResourceLoader();

	for (int n=0 ; n < numOfBullets ; n++)
	{

		BulletNode * pBullet = new BulletNode();
		ModelData * pBulletData = resourceLoader.LoadModel("bullet3.mod");
		pBullet->SetModelData(pBulletData);
		pBullet->SetSpeed(GLOBAL_SCALE*8.0f);
		pBullet->SetGravity(GLOBAL_SCALE*0.01f);
		pBullet->SetShootingRange(GLOBAL_SCALE*100.0f);
		pBullet->SetHitDistance(GLOBAL_SCALE*2.0f);
		mBulletManager.AddBulletNode(pBullet);		
	}


}

BulletNode * WeaponManager::Fire(bool noisy)
{	

	WeaponParams params = mCurrentWeapon->GetWeaponParams();
/*
	if (params.mCartridgeBulletsLeft == 0)
	{
		params.mCartridgeCount--;
		if (params.mCartridgeCount == 0 )
		{
			//SoundManager & soundManager = Soun dManager::GetSoundManager();
			//soundManager.SetSoundState( look for cartridge );
			return NULL;
		}

		params.mCartridgeBulletsLeft = params.mCartridgeSize;

	}

	params.mCartridgeBulletsLeft--;
*/
	mShootCount++;

	if (!(mShootCount%mShootingRate)) // for all weapons
	{
		BulletNode * pBulletNode = mBulletManager.EnableBulletNode();
		if (pBulletNode)
		{			
            pBulletNode->SetNoisy( noisy );
            
			Node * pShootingNode =  mCurrentWeapon->GetShootingNode();
			
			pBulletNode->SetShooterNode( pShootingNode );
			pBulletNode->SetDirectionNode( pShootingNode );
			
			return pBulletNode;
		}
	}

	return NULL;
}




