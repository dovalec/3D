#include "AnimationClipLogic.h"

NEWDEL_IMPL(AnimationClipLogic)
AnimationClipLogic::AnimationClipLogic(AnimationClip * pClip, int numOfLoops, float speed )
{
	numOfLoop = numOfLoops;
	mpClip = pClip;
	mSpeed = speed;
	mFramePos = 0.0f;
    
    mFirstFrame = 0;
    mSecondFrame = 1;
}

AnimationClipLogic::~AnimationClipLogic(void)
{
	
}


void AnimationClipLogic::ProgressAnimation(bool & finished)
{
    finished = false;
    
     Shell * pShell = Shell::GetShell();
    float fpsFactor = pShell->GetFPSFactor();
    
    mFramePos+=fpsFactor * mSpeed;//0.1f;
    if (mFramePos >= 1.0f )
    {
        mFirstFrame++;
        mSecondFrame++; 
        
        mFramePos = 0.0f;
    }
    
    if (mSecondFrame ==  mpClip->GetNumKeys() )
    {
        if (numOfLoop != INFINITE)
        {
            numOfLoop--;	
        }
        
        if (numOfLoop == 0)
        {
            finished = true;
        }
        
        
        ResetAnimationPos();
    }
}
