#ifndef __EventReceiver__
#define __EventReceiver__

#include "Event.h"

class EventReceiver {

public:
	NEWDEL_DECL
	EventReceiver();
	virtual ~EventReceiver()
	{
		LOGI("Removed receiver");
	}

public:
	virtual void HandleEvent(Event * pEvent) = 0;

};

#endif //__EventReceiver__
