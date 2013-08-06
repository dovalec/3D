#include "ScreenTouch.h"
#include "../Shell.h"


NEWDEL_IMPL(ScreenTouch)
ScreenTouch::ScreenTouch() 
{
	mPressed = false;
	mPosX = 0.0f;
	mPosY = 0.0f;
}

void ScreenTouch::OnTick() 
{	
    
	Shell * pShell = Shell::GetShell();
	int w, h;
	pShell->GetScreenSize(w,h);

	float screenRatio = pShell->GetScreenRatio();
	float one_screenRatio = 1.0f/screenRatio;

	float half_w = (float)w * 0.5f;
	float half_h = (float)h * 0.5f;


	float x1, y1, x2, y2;
	pShell->GetJoysticks(x1, y1, x2, y2);

	bool screenPressed = false;

	if (x1 > 0 || y1>0 )
		mPressed = true;
	else
		mPressed = false;

	mPosX = (x1 - half_w)/half_w;
	mPosY = (half_h - y1)/half_h;	

	//y1f *= one_screenRatio;

	
}

