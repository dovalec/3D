#ifndef __EventSender__
#define __EventSender__

#include "../Config.h"
#include "../TL/MyString.h"

class EventSender {

public:
	NEWDEL_DECL;
	EventSender(String type);

	inline String GetSenderType()
	{
		return mType;
	}

	void Enable();
	void Disable();

	inline bool IsEnabled()
	{
		return mEnabled;
	}

protected:
	String mType;
	bool mEnabled;

};

#endif //__EventSender__
