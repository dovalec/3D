#ifndef __EventManager__
#define __EventManager__

#include "Event.h"
#include "EventReceiver.h"
#include "../TL/List.h"

#define MAX_EVENT_RECEIVERS 256

class EventManager {
public:
	EventManager();
public:
	NEWDEL_DECL
	inline static EventManager & GetEventManager()
	{
		return *GlobalSys::GetEventManager();
	}

	void Destroy()
	{

	}

	void SendEvent(Event * pEvent);
	void SendEventNoDestroy(Event * pEvent);
	void AddEventReceiver(EventReceiver * pReceiver);
	void RemoveEventReceiver(EventReceiver * pReceiver);
	void RemoveAllReceivers();

	void OnTick();

	void ResetList();

protected:
	List mReceivers;

	List mEventsToHandle;
};

#endif //__EventManager__
