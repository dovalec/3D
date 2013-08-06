#include "JoyStick.h"
#include "../ResourceLoader.h"
#include "../Shell.h"

NEWDEL_IMPL(JoyStick)
JoyStick::JoyStick(void)
{
	mLeftJoystickPressed = false;
	mRightJoystickPressed = false;
		
	mLeftJoystickPos.x =  0.65f;
    mLeftJoystickPos.y =  -0.5f;
    
	mRightJoystickPos.x = 0.65f;
    mRightJoystickPos.y = -0.5f;
    
    mVectorRight.y = JOY_SCALE;
    
    
    mLeftVisible = true;
    mRightVisible = true;
    
}

JoyStick::~JoyStick(void)
{

}


bool JoyStick::Load()
{
	//LOGI("JoyStick::Load");

	ResourceLoader & resourceLoader = ResourceLoader::GetResourceLoader();
	
	mLeftJoy = resourceLoader.LoadOverlayFace("left_base.pvr", true);
	mRightJoy = resourceLoader.LoadOverlayFace("left_base.pvr", true);

	mJoyStickPointerLeftPressed = resourceLoader.LoadOverlayFace("red_btn.pvr", true);
	mJoyStickPointerLeftUnPressed = resourceLoader.LoadOverlayFace("red_btn.pvr", true);

	mJoyStickPointerRightPressed = resourceLoader.LoadOverlayFace("red_btn.pvr", true);
	mJoyStickPointerRightUnPressed = resourceLoader.LoadOverlayFace("red_btn.pvr", true);
	
/*
	mRightControls_0 = resourceLoader.LoadOverlayFace("wepon_change_unperssed.pvr");
	mRightControls_1 = resourceLoader.LoadOverlayFace("wepon_change_perssed1.pvr");
	mRightControls_2 = resourceLoader.LoadOverlayFace("wepon_change_perssed2.pvr");
	mRightControls_3 = resourceLoader.LoadOverlayFace("wepon_change_perssed3.pvr");
*/

	mLeftJoy->SetTransparent(true);
	mRightJoy->SetTransparent(true);
	mLeftJoy->SetLayer(3);
	mRightJoy->SetLayer(3);

	mJoyStickPointerLeftPressed->SetTransparent(true);
	mJoyStickPointerRightPressed->SetTransparent(true);
	mJoyStickPointerLeftPressed->SetLayer(3);
	mJoyStickPointerRightPressed->SetLayer(3);

	mJoyStickPointerLeftUnPressed->SetTransparent(true);
	mJoyStickPointerRightUnPressed->SetTransparent(true);
	mJoyStickPointerLeftUnPressed->SetLayer(3);
	mJoyStickPointerRightUnPressed->SetLayer(3);
/*
	mRightControls_0->SetTransparent(true);
	mRightControls_1->SetTransparent(true);
	mRightControls_2->SetTransparent(true);
	mRightControls_3->SetTransparent(true);

	mRightControls_0->SetLayer(3);
	mRightControls_1->SetLayer(3);
	mRightControls_2->SetLayer(3);
	mRightControls_3->SetLayer(3);
*/

	return true;
}

void JoyStick::Update()
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

	mLeftJoystickPressed = false;
	mRightJoystickPressed = false;

	if (x1 != 0 || y1 != 0)
		mLeftJoystickPressed = true;

	if (x2 != 0 || y2 != 0)
		mRightJoystickPressed = true;

	float x1f = (x1 - half_w)/half_w;
	float y1f = (half_h - y1)/half_h;	
	//y1f *= one_screenRatio;

	if (mLeftJoystickPressed && mLeftVisible)
	{
		mVectorLeft.x = x1f + mLeftJoystickPos.x;
		mVectorLeft.y = y1f - mLeftJoystickPos.y;	

        float lenLeft = mVectorLeft.length();
		if (lenLeft > JOY_SCALE*3.0f)
        {
            mLeftJoystickPressed = 0.0f; 
            mVectorLeft*=0.0f;
        }
        else if (lenLeft > JOY_SCALE)
		{
			mVectorLeft = mVectorLeft.normalize();
			mVectorLeft*=JOY_SCALE;
		}
        
      //  mVectorLeft = (mVectorLeft / JOY_SCALE);
	}
	else 
	{
		mVectorLeft*=0.0f;
	}
	
	float x2f = (x2 - half_w)/half_w;
	float y2f = (half_h - y2)/half_h;
    
    //y2f *= one_screenRatio;

	if (mRightJoystickPressed && mRightVisible)
	{
		
		mVectorRight.x = x2f - mRightJoystickPos.x;
		mVectorRight.y = y2f - mRightJoystickPos.y;
		
		float lenRight = mVectorRight.length();
        if (lenRight > JOY_SCALE*3.0f)
        {
            mRightJoystickPressed = 0.0f; 
            mVectorRight*=0.0f;
        }
        else if (lenRight > JOY_SCALE)
		{				
			mVectorRight.normalize();			
			mVectorRight*=JOY_SCALE;
		}
		
	}
	else 
	{		
		mVectorRight*=0.0f;
	}

	mLeftJoy->SetScale(JOY_SCALE, JOY_SCALE * screenRatio, 1.0f);
	mLeftJoy->SetTranslation(-mLeftJoystickPos.x, mLeftJoystickPos.y , 0);

	mRightJoy->SetScale(JOY_SCALE, JOY_SCALE * screenRatio, 1.0f);
	mRightJoy->SetTranslation(mRightJoystickPos.x , mRightJoystickPos.y , 0);

	mJoyStickPointerLeftPressed->SetScale(0.1f, 0.1f * screenRatio, 1.0f);
	mJoyStickPointerRightPressed->SetScale(0.1f, 0.1f * screenRatio, 1.0f);

	mJoyStickPointerLeftUnPressed->SetScale(0.1f, 0.1f * screenRatio, 1.0f);
	mJoyStickPointerRightUnPressed->SetScale(0.1f, 0.1f * screenRatio, 1.0f);
	

	mLeftJoy->Update();
	mRightJoy->Update();
	
	mJoyStickPointerLeftPressed->SetTranslation(mVectorLeft.x - mLeftJoystickPos.x , mVectorLeft.y * screenRatio + mLeftJoystickPos.y, 0);
	mJoyStickPointerLeftPressed->Update();

	mJoyStickPointerRightPressed->SetTranslation(mVectorRight.x + mRightJoystickPos.x , mVectorRight.y * screenRatio + mRightJoystickPos.y, 0);
	mJoyStickPointerRightPressed->Update();

	
	mJoyStickPointerLeftUnPressed->SetTranslation(mVectorLeft.x - mLeftJoystickPos.x , mVectorLeft.y * screenRatio + mLeftJoystickPos.y, 0);
	mJoyStickPointerLeftUnPressed->Update();

	mJoyStickPointerRightUnPressed->SetTranslation(mVectorRight.x + mRightJoystickPos.x , mVectorRight.y * screenRatio + mRightJoystickPos.y, 0);
	mJoyStickPointerRightUnPressed->Update();


	float cx = 0.67f;
/*
	mRightControls_0->SetTranslation( cx , 0, 0);
	mRightControls_0->Update();
	mRightControls_1->SetTranslation( cx , 0, 0);
	mRightControls_1->Update();
	mRightControls_2->SetTranslation( cx , 0, 0);
	mRightControls_2->Update();
	mRightControls_3->SetTranslation( cx , 0, 0);
	mRightControls_3->Update();
*/
}

void JoyStick::Apply()
{
	if (mLeftVisible)
    {
        mLeftJoy->Apply();
	
        if (mLeftJoystickPressed)
            mJoyStickPointerLeftPressed->Apply();
        else
            mJoyStickPointerLeftUnPressed->Apply();
    }
    
    if (mRightVisible)
    {
        mRightJoy->Apply();
        if (mRightJoystickPressed)
            mJoyStickPointerRightPressed->Apply();
        else
            mJoyStickPointerRightUnPressed->Apply();
    }

	//mRightControls_0->Apply();
}