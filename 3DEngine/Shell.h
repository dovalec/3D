#ifndef __Shell__
#define __Shell__

#include "Config.h"

class Shell {

public:	

	NEWDEL_DECL
    Shell();
    
	inline static Shell * GetShell()
	{
		return GlobalSys::GetShell();	
	}

	void Destroy()
	{

	}

	void InitShell();

	void SetReadPath(const char * pPath);
	void SetWritePath(const char * pPath);
	void SetCameraImage(const char * pPath);
	void SetSystem(const char * pSystem);
	void SetLowGPU(bool low);

	char * GetReadPath();
	char * GetWritePath();
	char * GetCameraImage();
	void ResetCameraImage();
	char * GetSystem();
	bool GetLowGPU();

	bool IsGLExtensionSupported(const char *extension);

	void SetScreenSize(int w, int h);

	inline void GetScreenSize(int & w, int & h)
	{
		w = mScreenWidth;
		h = mScreenHeight;
	}

	inline float GetScreenRatio()
	{
		return (float)mScreenWidth / (float)mScreenHeight;
	}
	inline void SetScreenRotated(bool rotate)
	{
		mScreenRotated = rotate;
	}

	inline bool GetScreenRotated()
	{
		return mScreenRotated;
	}

    inline void SetJoystickLeft(float x, float y)
	{
		mJoystickX1 = x;
		mJoystickY1 = y;
	} 
    
	inline void SetJoystickRight(float x, float y)
	{
		mJoystickX2 = x;
		mJoystickY2 = y;
	} 

	inline void GetJoysticks(float & x1, float & y1, float & x2, float & y2)
	{
		x1 = mJoystickX1;
		y1 = mJoystickY1;
		x2 = mJoystickX2;
		y2 = mJoystickY2;
	}

    
    inline float GetFrameTime()
    {
        return mFrameTime;
    }
    
    inline void SetFrameTime(float time)
    {
        mFrameTime = time;
        mFPSFactor = mFrameTime / GAME_FPS;
    }
    
    
    inline float GetFPSFactor()
    {
        return mFPSFactor;
    }
    
     
    inline void IncTickCount()
    {
        mTickCount++;
    }
    
    inline long GetTickCount()
    {
        return mTickCount;
    }

protected:
    
    long mTickCount;
    float mFrameTime;
    float mFPSFactor;
    
	char mSystem[256];
	char mWritePath[256];
	char mReadPath[256];
	char mCameraImage[256];
	int mScreenWidth;
	int mScreenHeight;
	bool mScreenRotated;

	float mJoystickX1;
	float mJoystickY1;
	float mJoystickX2;
	float mJoystickY2;

	bool mLowGpu;
};

#endif //__Shell__
