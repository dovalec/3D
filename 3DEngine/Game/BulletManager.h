#ifndef __BulletManager__
#define __BulletManager__

#include "../Config.h"
#include "../TL/List.h"

#include "BulletNode.h"

class BulletManager {

public:
	NEWDEL_DECL
	BulletManager();
	virtual ~BulletManager();
	
	void OnTick();

	
	void AddBulletNode(BulletNode * pBulletNode);
	int GetBulletNodeIndex(BulletNode * pBulletNode);
	
	BulletNode * EnableBulletNode();
	void DisableBulletNode(BulletNode * pBullet);

	void Enable();
	void Disable();

	inline List & GetIdleBulletList()
	{
		return mIdleBullets;
	}

	inline List & GetActiveBulletList()
	{
		return mActiveBullets;
	}

protected:
	List mIdleBullets;
	List mActiveBullets;

	bool mEnabled;
	
};

#endif //__BulletManager__
