#ifndef __BotNode__
#define __BotNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#include "MoveToTarget.h"
#include "GraphMoveToTarget.h"

#include "PlayerNode.h"

class BotManager;
class BotFsmMachine;

#define MAX_NUM_BOTS 32

class BotNode {
public:
	NEWDEL_DECL

	BotNode();
	virtual ~BotNode();
	
	virtual void OnTick();
	
	void SetSpeed(float speed);
	void SetShootingRange(float range);
	void SetHitDistance(float dist);
	
	virtual void SetModelData(ModelData * pModelData);
	inline ModelData * GetModelData()
	{
		return mModelData;
	}

	inline Node * GetNode()
	{
		return mModelData->GetRoot();
	}

	void Enable();
	void Disable();
	inline bool IsEnabled()
	{
		return mEnabled;
	}

	void SetPower(int power);
	int GetPower();

	void SetMachine(BotFsmMachine * pMachine);
	inline BotFsmMachine * GetMachine()
	{
		return mMachine;
	}

	void SetBotManager(BotManager * pBotManager);
	inline BotManager * GetBotManager()
	{
		return mBotManager;	
	}

	inline ModelData * GetPlayerModelData()
	{
		return mPlayerNode->GetModelData();
	}

	virtual void SetPlayerNode(PlayerNode * pPlayerNode);

	inline PlayerNode * GetPlayerNode()
	{
		return mPlayerNode;
	}

	inline MoveToTarget & GetMoveToTarget()
	{
		return mMoveToTarget;
	}

	inline GraphMoveToTarget & GetGraphMoveToTarget()
	{
		return mGraphMoveToTarget;
	}
    
    inline void SetOnlineMode(bool mode)
    {
        mOnlineMode = mode; 
    }
    
    inline void SetNetTickCount(float tickCount)
    {
        mNetDeltaTick = tickCount - mNetTickCount;
        LOGI("%f" , mNetDeltaTick);
        mNetTickCount = tickCount;
       
    }
    inline float GetNetDeltaTick()
    {
        return mNetDeltaTick;
    }
    
    inline bool GetOnlineMode()
    {
        return mOnlineMode;
    }

    inline void SetNetTranslation(float tx, float ty, float tz)
    {
       
        mPrevNetTranslation = mNewNetTranslation;
        mNewNetTranslation.x = tx;
        mNewNetTranslation.y = ty;
        mNewNetTranslation.z = tz;
        
        PVRTVec3 vecLen = mNewNetTranslation - mPrevNetTranslation;
        mNetWalkVec = vecLen * 0.100f;/// ( mNetDeltaTick / 0.100f);
    }
    
    inline void SetNetRotation(float ro)
    {        
        PVRTVECTOR3 axis = {0,1,0};
        PVRTMatrixQuaternionRotationAxis(mNetRotation, axis, ro);
    }
    
    inline PVRTVec3 GetPrevNetTranslation()
    {
        return mPrevNetTranslation;
    }
    
    inline PVRTVec3 GetNewNetTranslation()
    {
        return mNewNetTranslation;
    }

    inline PVRTQUATERNION GetNetRotation()
    {
        return mNetRotation;
    }

    
    inline PVRTVec3 GetNetWalkVec()
    {
        return mNetWalkVec;
    }
    
protected:
    /////////////// ONLINE ////////////////
    float mNetTickCount;
    float mNetDeltaTick;
    
    
    PVRTVec3 mNetWalkVec;
    
    PVRTVec3 mNewNetTranslation;
    PVRTVec3 mPrevNetTranslation;
    PVRTQUATERNION mNetRotation;
    bool mOnlineMode;
    /////////////// ONLINE ////////////////
    

	BotManager * mBotManager;
	BotFsmMachine * mMachine;
	ModelData * mModelData;

	PlayerNode * mPlayerNode;
	MoveToTarget mMoveToTarget;
	GraphMoveToTarget mGraphMoveToTarget; 
	
	float mRange;
	float mSpeed;
	
	float mHitDist;
	int mPower;
	bool mEnabled;

};

#endif //__BotNode__
