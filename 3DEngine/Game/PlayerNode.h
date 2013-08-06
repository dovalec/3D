#ifndef __PlayerNode__
#define __PlayerNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "../LocatorData.h"
#include "../ModelData.h"
#include "../Events/EventReceiver.h"
#include "BotFormation.h"
#include "StopWireUser.h"
#include "StopCircleUser.h"
#include "CollisionDataUser.h"
#include "DoorsDataUser.h"

#include "JoyStick.h"

#define MAX_TRIGGER_WIRES 256

class PlayerNode : public EventReceiver,
					public StopWireUser,
					public StopCircleUser,
					public CollisionDataUser,
					public DoorsDataUser 
{
public:
	NEWDEL_DECL
	PlayerNode();
	virtual ~PlayerNode();
	
	virtual void SetModelData(ModelData * pModelData);
	inline ModelData * GetModelData()
	{
		return mModelData;
	}

	inline Node * GetNode()
	{
		return mNode;
	}

	void SetStopOnFirstCollision(bool state);
	void SetSideScroller(bool sideScroll);
	

	bool CheckCollision();
	void SetWalkDistance(float dist);
	float GetWalkDistance();

	void OnTick();

	void AddTriggerWire(Wire * pWire);
	void RemoveTriggerWire(Wire * pWire);

	virtual void HandleEvent(Event * pEvent) = 0;

	inline void SetDontWalk(bool state)
	{
		mDontWalk = state;
	}
    
    
	inline void SetDontRotate(bool state)
	{
		mDontRotate = state;
	}
    
    
    inline void SetSmoothRotate(bool state)
    {
        mSmoothRotate = state;
    }


protected:
	PVRTMat4 mRotMtxLeft;
	PVRTMat4 mRotMtxRight;

	PVRTVec2 mLeftJoy;
	PVRTVec2 mRightJoy;
	PVRTVec2 mLastRightJoy;

	bool mDontWalk;
    bool mDontRotate;
    bool mSmoothRotate;
    
	float mWalkDistance;
	
	Node * mNode;
	ModelData * mModelData;

	int mNumTriggerWires;
	Wire * mTriggerWires[MAX_TRIGGER_WIRES];
	bool mStopOnFirstCollision;
	bool mLastRightJoystickPressed;
	

};

#endif //__PlayerNode__
