#ifndef __ProjectileExplosionNode__
#define __ProjectileExplosionNode__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../ModelData.h"

#define MAX_NUM_EXPLOSIONS 32

class ProjectileParticle
{
public:
    NEWDEL_DECL;
        
    ProjectileParticle();
    virtual ~ProjectileParticle();
    
    Mesh * mMesh;
    
    ///////////////////////////
    
    inline void SetProjectileAngle(float angle)
    {
        mProjectileAngle = angle;
    }
    
    inline void SetProjectileVelocity(float velocity)
    {
        if (mProjectileAngle == 0.0f)
        {
            STOP;
        }
        mVelocityX = velocity * cos(mProjectileAngle);
        mVelocityY = velocity * sin(mProjectileAngle);
    }
    
    inline void SetExplosionDirectionAngle(float angle)
    {
        mExplosionDirectionAngle = angle;
        PVRTMatrixRotationY(mExplosionDirectionMtx, mExplosionDirectionAngle);
    }
   
    inline Node * GetNode()
    {
        return mNode;
    }
    
    inline void SetNode(Node * pNode)
    {
        mNode = pNode;
    }

    
    virtual void ExplodeState();
	virtual void ExplodingState();
	
    
    float mExplosionDirectionAngle;
    float mProjectileAngle;
    float mGravity;
    
    float mVelocityX;
    float mVelocityY;
    
    PVRTMat4 mExplosionDirectionMtx;
	
    Node * mNode;
    
    
};

class ProjectileExplosionNode {
public:
	NEWDEL_DECL;
	
    enum ProjectileExplosionState {
		state_explode,
		state_exploding,
		state_finished,
        
	};
    
	ProjectileExplosionNode();
	virtual ~ProjectileExplosionNode();
	
	virtual void OnTick();
	
    void CreateParticles(int numOfPraticles);
    void InitParticles();
    
    void SetProjectileAngles(float min, float max)
    {
        mProjMinAngle = min;
        mProjMaxAngle = max;
    }
    
    inline ProjectileExplosionState GetCurrentState()
	{
		return mState;
	}
    
    virtual void ExplodeState();
    virtual void ExplodingState();
    
    void Enable();
	void Disable();
    
	inline bool IsEnabled()
	{
		return mEnabled;
	}
    
    inline void SetGlobalVelocity(float velocity)
    {
        mVelocity = velocity;
    }
    
    virtual void SetModelData(ModelData * pModelData);
    
    
    void SetExplosionPosition(PVRTVec3 pos)
    {
        mExplosionPos = pos;
    }
    
    void SetExplosionPosition(float x, float y, float z)
    {
        mExplosionPos.x = x;
        mExplosionPos.y = y;
        mExplosionPos.z = z;
        
    }
    
protected:
    
    ProjectileExplosionState mState;
	PVRTVec3 mExplosionPos;
    List mParticles;
    
    bool mEnabled;
    float mProjMinAngle;
	float mProjMaxAngle;
    float mVelocity;
    float mExplosionStartingTime;
    
    ModelData * mModelData;
};

#endif //__ProjectileExplosionNode__
