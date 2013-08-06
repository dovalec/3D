#ifndef __GunManager__
#define __GunManager__

#include "../Config.h"
#include "GunNode.h"

#include "../Events/EventManager.h"
#include "../Events/EventReceiver.h"

#define MAX_NUM_GUNS 256

class GunManager : public EventReceiver, public EventSender {
public:
	NEWDEL_DECL
	GunManager();
	virtual ~GunManager();
	
	void OnTick();
	
	void AddGunNode(GunNode * pGun);
	void RemoveGunNode(GunNode * pGun);

	virtual void HandleEvent(Event * pEvent);

protected:
	int mNumGunNodes;
	GunNode * mGunNodes[MAX_NUM_GUNS];
	
};

#endif //__GunManager__
