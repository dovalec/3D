#include "EventManager.h"

NEWDEL_IMPL(EventManager)
EventManager::EventManager()
{

}

void EventManager::SendEvent(Event * pEvent)
{
	pEvent->SetDestroy(true);
	mEventsToHandle.Add(pEvent);
	
}

void EventManager::SendEventNoDestroy(Event * pEvent)
{
	pEvent->SetDestroy(false);
	mEventsToHandle.Add(pEvent);
}

void EventManager::OnTick()
{
	Event * pEvent = NULL;
	while( pEvent = (Event*)mEventsToHandle.First() )
	{
		if (pEvent->GetSender()->IsEnabled())
		{
			ListIterator iter(mReceivers);
			while ( EventReceiver * pReceiver = (EventReceiver*)iter.GetPtr() )
			{
				pReceiver->HandleEvent(pEvent);
			}
		}
		if (pEvent->DoDestroy())
		{
			delete pEvent;
		}
		mEventsToHandle.RemoveFirst();
	}
}

void EventManager::AddEventReceiver(EventReceiver * pReceiver)
{
	mReceivers.Add(pReceiver);
}



void EventManager::RemoveEventReceiver(EventReceiver * pReceiver)
{
	mReceivers.Remove(pReceiver);
}

