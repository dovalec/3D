#include "Button.h"
#include "../Shell.h"

#include "../ResourceLoader.h"
#include "../Events/EventManager.h"

NEWDEL_IMPL(Button)
Button::Button() : EventSender("sender_ui_button")
{
	mLeftPressed = false;
	mRightPressed = false;
	mEventSender = this;
}

Button::~Button()
{
	SAFE_DEL(mPressedOverlay);
	SAFE_DEL(mReleasedOverlay);
}


void Button::LoadOverlays(String pressed, String released)
{
	ResourceLoader & resourceLoader = ResourceLoader::GetResourceLoader();

	mPressedOverlay = resourceLoader.LoadOverlayFace(pressed, true);
	mReleasedOverlay = resourceLoader.LoadOverlayFace(released, true);

	mPressedOverlay->SetTransparent(true);
	mReleasedOverlay->SetTransparent(true);

	mPressedOverlay->SetLayer(3);
	mReleasedOverlay->SetLayer(3);


}

void Button::SetPosition(float x, float y)
{ 
	mPosition.x = x;
	mPosition.y = y;
}

void Button::SetScale(float x, float y)
{
	mScale.x = x;
	mScale.y = y;
    
    mTouchScale = mScale;
    mReleaseScale = mScale;
}

void Button::SetTouchScale(float x, float y)
{
	mTouchScale.x = x;
	mTouchScale.y = y;
} 

void Button::SetReleaseScale(float x, float y)
{
	mReleaseScale.x = x;
	mReleaseScale.y = y;
} 


void Button::SetEventTypes(String pressed, String released)
{
	mPressedEvent = pressed;
	mReleasedEvent = released;
}

void Button::Draw() {
	
	Shell * shell = Shell::GetShell();

	float x1, y1, x2, y2;
	shell->GetJoysticks(x1, y1, x2, y2);

	int w, h;
	shell->GetScreenSize(w,h);
	float screenRatio = shell->GetScreenRatio();
   // screenRatio = 1.0f;

	CheckPressed(x1,y1, mLeftPressed);
	CheckPressed(x2,y2, mRightPressed);
	

	if (mLeftPressed || mRightPressed)
	{
		mPressedOverlay->SetScale(1.2f * mScale.x , 1.2f * mScale.y * screenRatio, 1.0f);
		mPressedOverlay->SetTranslation(mPosition.x, mPosition.y, 0);
		mPressedOverlay->Update();
		mPressedOverlay->Apply();	
	} 
	else 
	{
		mReleasedOverlay->SetScale(mScale.x , mScale.y * screenRatio , 1.0f);
		mReleasedOverlay->SetTranslation(mPosition.x, mPosition.y, 0);
		mReleasedOverlay->Update();
		mReleasedOverlay->Apply();
	}
	
}

void Button::CheckPressed(float x1, float y1, bool & mPressed)
{
	Shell * shell = Shell::GetShell();
	int w, h;
	shell->GetScreenSize(w,h);

	float screenRatio = shell->GetScreenRatio();
    screenRatio = 1.0f;
    
	float one_screenRatio = 1.0f/screenRatio;

	float half_w = (float)w * 0.5f;
	float half_h = (float)h * 0.5f;


	bool screenPressed = false;

	if (x1 > 0 || y1>0 )
		screenPressed = true;

	float x1f = (x1 - half_w)/half_w;
	float y1f = (half_h - y1)/half_h;	
	y1f *= one_screenRatio;

	
	if (screenPressed)
	{
		if (	x1f > (mPosition.x - mTouchScale.x) &&
				x1f < (mPosition.x + mTouchScale.x) &&
				y1f > (mPosition.y - mTouchScale.y) &&
				y1f < (mPosition.y + mTouchScale.y)
			)
				
		{
			if (!mPressed)
			{
				mPressed = true;
				EventManager  & eventManager = EventManager::GetEventManager();
				eventManager.SendEvent( new Event(mEventSender, mPressedEvent) );
			}
		}	
		else if (x1f < (mPosition.x - mReleaseScale.x) ||
                 x1f > (mPosition.x + mReleaseScale.x) ||
                 y1f < (mPosition.y - mReleaseScale.y) ||
                 y1f > (mPosition.y + mReleaseScale.y)
                 )
		{
			if (mPressed)
			{		
                mPressed = false;			
				EventManager  & eventManager = EventManager::GetEventManager();
				eventManager.SendEvent( new Event(mEventSender, mReleasedEvent) );
			}
		}
		
	}
	else
	{
		if (mPressed)
		{
			mPressed = false;
			EventManager  & eventManager = EventManager::GetEventManager();
			eventManager.SendEvent( new Event(mEventSender, mReleasedEvent) );
		}
	}

}

