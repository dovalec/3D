#include "FollowPlayerNode.h"
#include "../AI/Collision.h"
#include "../Shell.h"

NEWDEL_IMPL(FollowPlayerNode)
FollowPlayerNode::FollowPlayerNode()
{
	mMaxDist = GLOBAL_SCALE*6.0f;
}

FollowPlayerNode::~FollowPlayerNode()
{

}

void FollowPlayerNode::CalcNodePos()
{
	
	PVRTVec3 targetWorldPos = mTargetNode->GetWorldTranslation();	
	PVRTVec3 nodeWorldPos = mNode->GetWorldTranslation();	
	
	PVRTVec3 nodeNewWorldPos(targetWorldPos.x + (mFront * mMaxDist * 0.5f), targetWorldPos.y, targetWorldPos.z + (mFront * mMaxDist));

	const float avgSize = 20.0f;
	float newX = ((nodeWorldPos.x * (avgSize-1.0f))/avgSize) + (nodeNewWorldPos.x)/avgSize;
	float newY = ((nodeWorldPos.y * (avgSize-1.0f))/avgSize) + (nodeNewWorldPos.y)/avgSize;
	float newZ = ((nodeWorldPos.z * (avgSize-1.0f))/avgSize) + (nodeNewWorldPos.z)/avgSize;

	

	PVRTVec3 dir = nodeWorldPos - targetWorldPos;
	float rotAngle = atan2f(dir.z, dir.x) + PVRT_PI*0.5f;

	PVRTQUATERNION quat;
	PVRTMatrixQuaternionRotationAxis(quat, PVRTVec3(0,1,0),rotAngle);
	mNode->SetTranslation(newX, newY, newZ);
	mNode->SetRotation(quat);
	//mNode->SetScale(0.3f, 0.3f, 0.3f);

	//mNode->UpdateInverseWorldTransform();

	//mNode->SetTranslation(newX, newY, newZ);
	
}
