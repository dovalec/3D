#include "BotFormation.h"
#include "BotNode.h"
#include "PlayerNode.h"

NEWDEL_IMPL(BotFormation)
BotFormation::BotFormation()
{
	mNumBotNodes = 0;
	mMethod = horizontal_line_mode;


	mBoxParams.width = GLOBAL_SCALE*5.0f;
	mBoxParams.depth = GLOBAL_SCALE*5.0f;
	mBoxParams.distance = GLOBAL_SCALE*5.0f;

	mCircleParams.radius = GLOBAL_SCALE*5.0f;
	mCircleParams.distance = GLOBAL_SCALE*5.0f;

	mLineParams.length = GLOBAL_SCALE*10.0f;
	mLineParams.distance = GLOBAL_SCALE*4.0f;


	mLastPlayerWorldPos *= 0.0f;
	mPlayerMoveAngle = 0.0f;

	mGroupSize = 3;

	for (int n=0;n<MAX_GROUP_SIZE;n++)
	{
		mRandomFactor[n]  = ((float(rand()) / float(RAND_MAX)) * (1.5f - 0.5f)) + 0.5f;
	}
}

BotFormation::~BotFormation()
{
	
}

void BotFormation::SetGroupSize(int size)
{
	mGroupSize = size;
}

bool BotFormation::AddBotNode(BotNode * pBotNode)
{
	if (IsGroupFull())
		return false;

	if (mNumBotNodes == MAX_GROUP_SIZE)
	{
		LOGE("mNumBotNodes == MAX_GROUP_SIZE");
		STOP;
	}

	mBotNodes[mNumBotNodes] = pBotNode;
	mNumBotNodes++;

	return true;
}

void BotFormation::RemoveBotNode(BotNode * pBotNode)
{
	for (int n=0;n<mNumBotNodes;n++)
	{
		if (mBotNodes[n] == pBotNode)
		{
			mNumBotNodes--;
			mBotNodes[n] = mBotNodes[mNumBotNodes];
			break;
		}
	}
}

void BotFormation::RemoveBotNode(int index)
{
	mNumBotNodes--;
	mBotNodes[index] = mBotNodes[mNumBotNodes];
}

void BotFormation::OnTick()
{
	if (mNumBotNodes == 0)
		return;

	switch (mMethod)
	{
		case box_mode:
			BoxMode();
			break;
		case circle_mode:
			CircleMode();
			break;
		case diagonal_line_mode:
			DiagonalLineMode();
		case vertical_line_mode:
			VerticalLineMode();
		case horizontal_line_mode:
			HorizontalLineMode();
			break;


	}
}

void BotFormation::BoxMode()
{
	
}

void BotFormation::CircleMode()
{

	Node * playerNodeRoot = mPlayerNode->GetNode();
	PVRTVec3 playerWordlPos = playerNodeRoot->GetWorldTranslation();

	float circleAngle = (PVRT_PI*2)/(float)mNumBotNodes;
	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n].x = mCircleParams.radius * cos(circleAngle*(float)n);
		mTargetPoints[n].y = 0;
		mTargetPoints[n].z = mCircleParams.radius * sin(circleAngle*(float)n);
	}

	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n] += playerWordlPos;		
		mBotNodes[n]->GetMoveToTarget().SetTargetPos(mTargetPoints[n]);
		mBotNodes[n]->GetMoveToTarget().SetLookAtTargetPos(playerWordlPos);
	}

}

void BotFormation::DiagonalLineMode()
{
	
	LineMode(PVRT_PI*0.25f);
}

void BotFormation::HorizontalLineMode()
{

	LineMode(PVRT_PI);
}

void BotFormation::VerticalLineMode()
{

	float delta = mLineParams.length / (float)mNumBotNodes;

	Node * playerNodeRoot = mPlayerNode->GetNode();
	PVRTVec3 playerWordlPos = playerNodeRoot->GetWorldTranslation();
/*
	PVRTVec3 playerMoveDir = playerWordlPos - mLastPlayerWorldPos;
	if (playerMoveDir.lenSqr() > MM(1.0f))
	{
		playerMoveDir.normalize();
		mPlayerMoveAngle = addAngle + atan2f(playerMoveDir.x, playerMoveDir.z);
		mLastPlayerWorldPos = playerWordlPos;
	}
*/

	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n].x = mRandomFactor[n] * ((float)n * delta) * 0.5f;
		mTargetPoints[n].y = 0;
		mTargetPoints[n].z = mRandomFactor[n] * ((float)n * delta);
	}

	PVRTMat4 rotMtx;
	PVRTMatrixRotationY(rotMtx, 0);

	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n] *= rotMtx;
		mTargetPoints[n] += playerWordlPos;
		mBotNodes[n]->GetMoveToTarget().SetTargetPos(mTargetPoints[n]);
		mBotNodes[n]->GetMoveToTarget().SetLookAtTargetPos(playerWordlPos);
	}

}

void BotFormation::LineMode(float addAngle)
{
	float delta = mLineParams.length / (float)mNumBotNodes;
	float lengthDiv2 = mLineParams.length*0.5f;


	Node * playerNodeRoot = mPlayerNode->GetNode();
	PVRTVec3 playerWordlPos = playerNodeRoot->GetWorldTranslation();
/*
	PVRTVec3 playerMoveDir = playerWordlPos - mLastPlayerWorldPos;
	if (playerMoveDir.lenSqr() > MM(1.0f))
	{
		playerMoveDir.normalize();
		mPlayerMoveAngle = addAngle + atan2f(playerMoveDir.x, playerMoveDir.z);
		mLastPlayerWorldPos = playerWordlPos;
	}
*/

	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n].x = mRandomFactor[n] * ((float)n * delta) - lengthDiv2;
		mTargetPoints[n].y = 0;
		mTargetPoints[n].z = mLineParams.distance * mRandomFactor[n];
	}

	PVRTMat4 rotMtx;
	PVRTMatrixRotationY(rotMtx, addAngle);

	for (int n=0;n<mNumBotNodes;n++)
	{
		mTargetPoints[n] *= rotMtx;
		mTargetPoints[n] += playerWordlPos;
		mBotNodes[n]->GetMoveToTarget().SetTargetPos(mTargetPoints[n]);
		mBotNodes[n]->GetMoveToTarget().SetLookAtTargetPos(playerWordlPos);
	}

}

void BotFormation::SetFormationMethod(BotFormationMethod method)
{
	mMethod = method;
}

void BotFormation::SetPlayerNode( PlayerNode * playerNode)
{
	mPlayerNode = playerNode;
}

void BotFormation::SetBoxParams(float width, float depth, float distance)
{
	mBoxParams.width = width;
	mBoxParams.depth = depth;
	mBoxParams.distance = distance;
}

void BotFormation::SetCircleParams(float radius, float distance )
{
	mCircleParams.radius = radius;
	mCircleParams.distance = distance;
}

void BotFormation::SetLineParams(float length, float distance)
{
	mLineParams.length = length;
	mLineParams.distance = distance;
}


