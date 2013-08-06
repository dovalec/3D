#ifndef __AnimationClipLogic__
#define __AnimationClipLogic__

#include "../Config.h"
#include "AnimationKeys.h"
#include "AnimationClip.h"
#include "../Shell.h"

class AnimationClipLogic
{
public:
	NEWDEL_DECL
	AnimationClipLogic(AnimationClip * pClip, int numOfLoops, float speed );
	virtual ~AnimationClipLogic();

	inline AnimationClip * GetClip()
	{
		return mpClip;
	}

	inline int GetLoopCount()
	{
		return numOfLoop;
	}

	inline float GetSpeed()
	{
		return mSpeed;
	}

	inline void SetSpeed(float speed)
	{
		mSpeed = speed;
	}

	
/*

	inline void ProgressAnimation(bool & finished)
	{
		finished = false;
		//float keyDuration = mpClip->GetDuration() / (float)mpClip->GetNumKeys();
		mAnimationPos += mSpeed;

		//if ( mAnimationPos >= mpClip->GetDuration() )
		if ( mAnimationPos >= (float)( mpClip->GetNumKeys()-1) )
		{
			if (numOfLoop != INFINITE)
			{
				numOfLoop--;	
			}

			if (numOfLoop == 0)
			{
				finished = true;
			}
			

			mAnimationPos = 0.0f;		
            //mAnimationPos -= (float)( mpClip->GetNumKeys()-1);
		}
	}
*/
    inline void ResetAnimationPos()
	{
		mFramePos = 0.0f;
        mFirstFrame = 0;
        mSecondFrame = 1;
	}
    
    
    inline float GetFramePos()
	{
		return mFramePos;
	}
    
    inline int GetFirstFrame()
    {
        return mFirstFrame;
    }
    
    inline int GetSecondFrame()
    {
        return mSecondFrame;
    }
    
    void ProgressAnimation(bool & finished);

private:
	AnimationClip * mpClip;
	unsigned int  numOfLoop;
    int mFirstFrame;
    int mSecondFrame;
    
	float mSpeed;
	float mFramePos;
};



#endif // __AnimationClipLogic__