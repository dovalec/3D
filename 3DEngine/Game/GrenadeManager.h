#ifndef __GrenadeManager__
#define __GrenadeManager__

#include "../Config.h"
#include "../TL/List.h"

#include "GrenadeNode.h"

class GrenadeManager {

public:
	NEWDEL_DECL
	GrenadeManager();
	virtual ~GrenadeManager();
	
	void OnTick();

	
	void AddGrenadeNode(GrenadeNode * pGrenadeNode);
	int GetGrenadeNodeIndex(GrenadeNode * pGrenadeNode);
	
	GrenadeNode * EnableGrenadeNode();
	void DisableGrenadeNode(GrenadeNode * pGrenade);

	void Enable();
	void Disable();

	inline List & GetIdleGrenadeList()
	{
		return mIdleGrenades;
	}

	inline List & GetActiveGrenadeList()
	{
		return mActiveGrenades;
	}

protected:
	List mIdleGrenades;
	List mActiveGrenades;

	bool mEnabled;
	
};

#endif //__GrenadeManager__
