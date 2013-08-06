#ifndef __ScreenTouch__
#define __ScreenTouch__

#include "../Config.h"

class ScreenTouch 
{
private:
	ScreenTouch();
public:
	NEWDEL_DECL
	
	inline static ScreenTouch & GetScreenTouch()
	{
		static ScreenTouch touch;
		return touch;
	}


	virtual void OnTick();
	
	inline bool GetTouch(float & x, float & y)
	{
		x = mPosX;
		y = mPosY;
		return mPressed;
	}

	

protected:
	bool mPressed;
	float mPosX;
	float mPosY;
};

#endif //__ScreenTouch__
