#ifndef __Event__
#define __Event__

#include "../Config.h"
#include "../Shell.h"

#include "EventSender.h"

class Event {

public:
	NEWDEL_DECL;
	Event(EventSender * pSender, String type);

	inline EventSender * GetSender()
	{
		return mSender;
	}


	inline String GetType()
	{
		return mType;
	}

	inline bool DoDestroy()
	{
		return mDestroy;
	}
	inline void SetDestroy(bool destroy)
	{
		mDestroy = destroy;
	}

protected:
	bool mDestroy;
	String mType;
	EventSender * mSender;

};

#endif //__Event__
