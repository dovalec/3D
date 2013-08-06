#ifndef __BombManager__
#define __BombManager__

#include "../Config.h"
#include "BombNode.h"
#include "CollisionManager.h"

#include "../Events/EventManager.h"
#include "../Events/EventReceiver.h"

class BombManager : public EventReceiver {

public:
	NEWDEL_DECL;

	enum BombManagerCollision {
		collision_shooter_bombs,
		collision_bombs_targets,
		collision_last,
	};

	BombManager();
	virtual ~BombManager();
	
	void OnTick();

	inline int GetNumEnabledBombs()
	{
		return mNumEnabledBombs;
	}

	inline int GetNumBombs()
	{
		return mNumBombNodes;
	}

	inline BombNode * GetBombNode(int n)
	{
		return mBombNodes[n];
	}

	inline CollisionManager & GetCollisionManager()
	{
		return mCollisionManager;
	}

	void AddBombNode(BombNode * pBombNode);
	int GetBombNodeIndex(BombNode * pBombNode);
	void RemoveBombNode(BombNode * pBombNode);
	void RemoveBombNode(int index);

	void EnableAllBombs();
	BombNode * EnableBombNode();
	void DisableBombNode(int index);
	void DisableBombNode(BombNode * pBombNode);

	virtual void HandleEvent(Event * pEvent);

protected:
	int mNumBombNodes;
	int mNumEnabledBombs;
	BombNode * mBombNodes[MAX_NUM_BOMBS];

	CollisionManager mCollisionManager;
	
};

#endif //__BombManager__
