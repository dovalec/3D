#ifndef __JoyStick__
#define __JoyStick__

#include "../OverlayFace.h"
#include "../GlobalHeap.h"

#define JOY_SCALE 0.3f
//#define JOY_MAX_LEN 0.424264f

class JoyStick : public Node
{
public:
	NEWDEL_DECL;
	

	JoyStick(void);
	virtual ~JoyStick(void);

    static inline JoyStick * GetJoyStick()
    {
        return GlobalHeap::GetJoyStick();    
    }
    
    
	virtual bool Load();
	virtual void Update();
	virtual void Apply();
	
	const PVRTVec2 & GetVectorLeft()
	{
		return mVectorLeft;
	}
	const PVRTVec2 & GetVectorRight()
	{
		return mVectorRight;
	}

	inline bool IsRightJoystickPressed()
	{
		return mRightJoystickPressed;
	}

	inline bool IsLeftJoystickPressed()
	{
		return mLeftJoystickPressed;
	}

    void SetLeftVisible(bool state)
    {
        mLeftVisible = state;
    }
    
    void SetRightVisible(bool state)
    {
        mRightVisible = state;
    }
    
private:

	PVRTVec2 mLeftJoystickPos;
	PVRTVec2 mRightJoystickPos;
    
	bool mRightJoystickPressed;
	bool mLeftJoystickPressed;
    
    bool mLeftVisible;
    bool mRightVisible;
    

	PVRTVec2 mVectorLeft;
	PVRTVec2 mVectorRight;

	OverlayFace * mLeftJoy;
	OverlayFace * mRightJoy;

	OverlayFace * mJoyStickPointerLeftPressed;
	OverlayFace * mJoyStickPointerRightPressed;

	OverlayFace * mJoyStickPointerLeftUnPressed;
	OverlayFace * mJoyStickPointerRightUnPressed;

	OverlayFace * mRightControls_0;
	OverlayFace * mRightControls_1;
	OverlayFace * mRightControls_2;
	OverlayFace * mRightControls_3;
};

#endif //__JoyStick__