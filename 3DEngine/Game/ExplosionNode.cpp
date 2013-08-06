#include "ExplosionNode.h"
#include "../TLSF/Tlsf.h"

NEWDEL_IMPL(ExplosionNode)

ExplosionNode::ExplosionNode()
{
	mShooterNode = NULL;
	mTargetNode = NULL;
	mModelData = NULL;
	mState = state_explode;
	mSpeed = GLOBAL_SCALE*1.0f;
	mRange = GLOBAL_SCALE*50.0f;

	mFriction = GLOBAL_SCALE*0.0f;
	mGravity = GLOBAL_SCALE*0.0f;
	mHitDist = GLOBAL_SCALE*0.01f;
	mEnabled = false;

	mTextureRows = 5;
	mTextureCols = 5;

	mCurrentRow = 0;
	mCurrentCol = 0;

	mSubImageCount = 25;

	mPower = 5;

	mMesh = NULL;

}

ExplosionNode::~ExplosionNode()
{

}

void ExplosionNode::SetHitDistance(float dist)
{
	mHitDist = dist;
}
void ExplosionNode::SetSpeed(float speed)
{
	mSpeed = speed;
}

void ExplosionNode::SetRange(float range)
{
	mRange = range;
}


void ExplosionNode::SetFriction(float friction)
{
	mFriction = friction;
}


void ExplosionNode::SetTextureRows(int rows)
{
	mTextureRows = rows;
}

void ExplosionNode::SetTextureCols(int cols)
{
	mTextureCols = cols;
}

void ExplosionNode::SetSubImageCount(int count)
{
	mSubImageCount = count;
}

void ExplosionNode::SetModelData(ModelData * pModelData)
{
	mModelData = pModelData;
}

void ExplosionNode::SetShooterNode(Node * pNode)
{
	mShooterNode = pNode;
}

void ExplosionNode::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}

void ExplosionNode::SetPower(int power)
{
	mPower = power;
}

int ExplosionNode::GetPower()
{
	return mPower;
}

void ExplosionNode::ExplodeState()
{
	mCurrentRow = 0;
	mCurrentCol = 0;

	mExplosionPos = mShooterNode->GetWorldTranslation();
	mExplosionPos.y = 2;// !!!!!!!!!!!!!!
	mExplosionStartingPos = mExplosionPos;

	PVRTQUATERNION shooterQuat = mShooterNode->GetRotation();

	PVRTMATRIX shooterRotaionMtx;
	PVRTMatrixRotationQuaternion(shooterRotaionMtx, shooterQuat);

	PVRTMat4 mat(shooterRotaionMtx.f);
	PVRTVec4 zVec(0, 0 , 1.0f, 1.0f);
	zVec = mat * zVec;


/*
	if (mAutoAim)
	{
		mDir = mExplosionPos - mTargetNode->GetWorldTranslation();
		mDir.normalize();
	}
*/	
	mDir.x = zVec.x;
	mDir.y = zVec.y;
	mDir.z = zVec.z;

	//mDir.z = 0;
	//mDir.x = -1;

	mState = state_exploding;

}

void ExplosionNode::ExplodingState()
{
	 mExplosionPos += mDir * mSpeed;
	 mExplosionPos.y -= mGravity;
	 mSpeed -= mFriction;

	 PVRTVec3 toTarget = mExplosionPos - mTargetNode->GetWorldTranslation();
	 float dist = toTarget.lenSqr();
	 if (dist < MM(mHitDist))
	 {
		mState = state_hit;
	 }
	 else if (mSpeed < 0.0f || mExplosionPos.y < 0.0f)
	 {
		 mState = state_missed;
	 }
	 else
	 {
		PVRTVec3 fromStart = mExplosionPos - mExplosionStartingPos;
		float fromStartDist = fromStart.lenSqr();
		if (fromStartDist > MM(mRange)) // acts like bullet - its a copy - we dont need it for explosions
		{
			mState = state_missed;
		}
	 }

	mCurrentCol++;
	if (mCurrentCol == mTextureCols)
	{
		mCurrentCol = 0;
		mCurrentRow++;
	}

	if ( ((mTextureCols * mCurrentRow) + mCurrentCol) >= mSubImageCount )
	{
		mState = state_missed;
		mCurrentCol = 0;
		mCurrentRow = 0;
	}
	
	if (mState == state_exploding)
	{
		PVRTMat4 uvTransMtx;
		PVRTMatrixTranslation(uvTransMtx, (float)mCurrentCol*(0.2f), 0.0f, (float)mCurrentRow*(0.2f));

		mMesh->GetAppearance().SetUVMatrix(uvTransMtx);
	}

}

void ExplosionNode::HitState()
{
	mState = state_finished;
}

void ExplosionNode::MissedStat()
{
	mState = state_finished;
}


void ExplosionNode::OnTick()
{
	switch (mState)
	{
		case state_explode:
			ExplodeState();
			break;
		case state_exploding:
			ExplodingState();
			break;
		case state_hit:
			HitState();
			break;
		case state_missed:
			MissedStat();
			break;

	}
/*
	PVRTVec3 playerWorldPos = mPlayerNode->GetNode()->GetWorldTranslation();
	PVRTVec3 locatorPos(mLocator->x, mLocator->y, mLocator->z);
	PVRTVec3 distVec = playerWorldPos - locatorPos;
	if (distVec.lenSqr() < MM(mLocator->radius))
	{
		ExplodeState();
	}
	else
	{
		IdleState();
	}
*/
	mModelData->GetRoot()->SetTranslation(mExplosionPos);
}


void ExplosionNode::Enable()
{
	mEnabled = true;
	mState = state_explode;
}

void ExplosionNode::Disable()
{
	mEnabled = false;
}

