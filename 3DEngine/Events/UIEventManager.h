#ifndef __EventManager__
#define __EventManager__

#include "Event.h"
#include "EventReceiver.h"
#include "../TL/List.h"

#define MAX_EVENT_RECEIVERS 256

class EventManager {
private:
	EventManager();
public:
	NEWDEL_DECL
	inline static EventManager & GetEventManager()
	{
		static EventManager manager;
		return manager;
	}

	void Destroy()
	{

	}

	void SendEvent(Event * pEvent);
	void SendEventNoDestroy(Event * pEvent);
	void AddEventReceiver(EventReceiver * pReceiver);
	void RemoveEventReceiver(EventReceiver * pReceiver);

	void OnTick();

protected:
	List mReceivers;

	List mEventsToHandle;
};

#endif //__EventManager__
