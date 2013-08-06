#ifndef __BulletManager__
#define __BulletManager__

#include "../Config.h"
#include "BulletNode.h"

class BulletManager {
private:
	BulletManager();

public:
	NEWDEL_DECL
	virtual ~BulletManager();
	
	inline static BulletManager & GetBulletManager()
	{
		static BulletManager manager;
		return manager;
	}

	void Update();
	void Apply();

	void AddBulletNode(BulletNode * pBulletNode);
	void RemoveBulletNode(BulletNode * pBulletNode);
	void RemoveBulletNode(int index);

	void EnableBulletNode(int index);
	void DisableBulletNode(int index);


protected:
	int mNumBulletNodes;
	BulletNode * mBulletNodes[MAX_NUM_BULLETS];
	
};

#endif //__BulletManager__
