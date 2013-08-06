#include "EventSender.h"

NEWDEL_IMPL(EventSender)
EventSender::EventSender(String type)
{
	mType = type;
	mEnabled = true;
}

void EventSender::Enable()
{
	mEnabled = true;
}

void EventSender::Disable()
{
	mEnabled = false;
}