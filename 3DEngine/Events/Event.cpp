#include "Event.h"


NEWDEL_IMPL(Event)
Event::Event(EventSender * pSender, String type)
{
	mSender = pSender;
	mType = type;
	mDestroy = true;
}

