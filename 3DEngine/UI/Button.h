#ifndef __Button__
#define __Button__

#include "../Config.h"
#include "../OverlayFace.h"
#include "../Events/EventSender.h"
#include "../Events/EventReceiver.h"

class Button : public EventSender
{

public:
	NEWDEL_DECL
	Button();
	virtual ~Button();

	virtual void Draw();
	void CheckPressed(float x1, float y1, bool & mPressed);
	
	void LoadOverlays(String pressed, String released);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
    void SetTouchScale(float x, float y);
    void SetReleaseScale(float x, float y);
    
	void SetEventTypes(String pressed, String released);
	void SetEventSender(EventSender * pEventSender)
	{
		mEventSender = pEventSender;
	}

	OverlayFace * GetPressedOverlay()
	{
		return mPressedOverlay;
	}
	OverlayFace * GetReleasedOverlay()
	{
		return mReleasedOverlay;
	}

protected:
	PVRTVec2 mPosition;
	PVRTVec2 mScale;
    PVRTVec2 mTouchScale;
    PVRTVec2 mReleaseScale;

	OverlayFace * mPressedOverlay;
	OverlayFace * mReleasedOverlay;

	String mPressedEvent;
	String mReleasedEvent;
	
	bool mLeftPressed;
	bool mRightPressed;

	EventSender * mEventSender;

};

#endif //__Button__
