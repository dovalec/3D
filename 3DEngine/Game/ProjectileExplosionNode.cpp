#include "ProjectileExplosionNode.h"
#include "../TLSF/Tlsf.h"

NEWDEL_IMPL(ProjectileExplosionNode)

ProjectileParticle::ProjectileParticle()
{
    
    mExplosionDirectionAngle = 0.0f;
    mProjectileAngle = 0.0f;

    mGravity = 9.8f;
    mVelocityX = 0.0f;
    mVelocityY = 0.0f;
    
    mNode = NULL;
}



//////////////////////////////////////////////////////////////////

ProjectileExplosionNode::ProjectileExplosionNode()
{
	mExplosionPos *= 0.0f;
    
    mProjMinAngle = PVRT_PI * 0.5f * 0.1f;
    mProjMaxAngle = PVRT_PI * 0.5f * 0.95f;
   
    mVelocity = 0.0f;
    mEnabled = false;

    mModelData = NULL;
}

ProjectileExplosionNode::~ProjectileExplosionNode()
{
    
}

void ProjectileExplosionNode::OnTick()
{
    Shell * pShell = Shell::GetShell();
    float time = pShell->GetFrameTime() - mExplosionStartingTime;
      
	ListIterator iter(mParticles);
    
    while (ProjectileParticle * pParticle = (ProjectileParticle*)iter.GetPtr())
	{
       
        float ypos = pParticle->mVelocityY * time + 0.5f * pParticle->mGravity * (time*time);
        float xpos = pParticle->mVelocityX * time + 0.5f * 0.0f * (time*time);
        
        PVRTVec4 zVec(0, ypos , xpos, 1.0f);
        zVec = pParticle->mExplosionDirectionMtx * zVec;
        
        PVRTVec3 particleWorldPos;
        
        particleWorldPos.x = zVec.x;
        particleWorldPos.y = zVec.y;
        particleWorldPos.z = zVec.z;
        
        particleWorldPos += mExplosionPos;
        pParticle->GetNode()->SetTranslation(particleWorldPos);
	}	
    
    switch (mState)
	{
		case state_explode:
			ExplodeState();
			break;
		case state_exploding:
			ExplodingState();
			break;
            
	}
  
    mModelData->GetRoot()->SetTranslation(mExplosionPos);
}

void ProjectileExplosionNode::CreateParticles(int numOfPraticles)
{
    for (int n=0;n<numOfPraticles;n++)
    {
        mParticles.Add(new ProjectileParticle());
    }
}

void ProjectileExplosionNode::InitParticles()
{
    
    ListIterator iter(mParticles);
    
    while (ProjectileParticle * pParticle = (ProjectileParticle*)iter.GetPtr())
	{
        float particleExplosionDirectionAngle = ((float(rand()) / float(RAND_MAX)) * (PVRT_PI*2.0f - 0.0f)) + 0.0f;
        float projectileAngle = ((float(rand()) / float(RAND_MAX)) * (mProjMaxAngle - mProjMinAngle)) + mProjMinAngle;
        float particleVelocity = ((float(rand()) / float(RAND_MAX)) * (mVelocity - mVelocity*0.5f)) + mVelocity*0.5f;
        
		pParticle->SetExplosionDirectionAngle(particleExplosionDirectionAngle);
        pParticle->SetProjectileAngle( projectileAngle );
        pParticle->SetProjectileVelocity( particleVelocity );
	}	    
}

void ProjectileExplosionNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
}

void ProjectileExplosionNode::ExplodeState()
{
    Shell * pShell = Shell::GetShell();
    mExplosionStartingTime = pShell->GetFrameTime();
    
    InitParticles();
}

void ProjectileExplosionNode::ExplodingState()
{
    
}

void ProjectileExplosionNode::Enable()
{
	mEnabled = true;
	mState = state_explode;
}

void ProjectileExplosionNode::Disable()
{
	mEnabled = false;
}



