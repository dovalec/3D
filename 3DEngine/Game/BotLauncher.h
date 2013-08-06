#ifndef __BotLauncher__
#define __BotLauncher__

#include "../Config.h"

#include "BotManager.h"


class BotLauncher {

public:
	NEWDEL_DECL
	BotLauncher();
	virtual ~BotLauncher();

	void SetPosition(PVRTVec3 pos);
	void SetPosition(float x, float y, float z);
	void SetRadius(float radius);
	String GetName();
	void SetName(String name);

	inline float GetRadius()
	{
		return mRadius;
	}

	inline const PVRTVec3 & GetPosition()
	{
		return mPosition;
	}
/*
	inline BotManager & GetBotManager()
	{
		return mBotManager;
	}
*/

	void Transform(PVRTMat4 mtx);

protected:
	PVRTVec3 mPosition;
	//BotManager mBotManager;
	float mRadius;
	String mName;
};

#endif //__BotLauncher__
