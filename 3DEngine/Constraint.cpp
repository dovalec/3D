#include "Constraint.h"
#include "Shell.h"

NEWDEL_IMPL(Constraint)
Constraint::Constraint()
{
	
	mTargetNode = NULL;
	mNode = NULL;

	mConstraintType = CONSTRAINT_TYPE_MTX;
	mConstraintShift *= 0.0f;
	
}

Constraint::~Constraint()
{

}

void Constraint::SetTargetNode(Node * pNode)
{
	mTargetNode = pNode;
}


void Constraint::SetNode(Node * pNode)
{
	mNode = pNode;
//	mNode->SetExternalUpdateMode(true);
}


void Constraint::OnTick()
{
	switch (mConstraintType)
	{
		case CONSTRAINT_TYPE_MTX:
			mNode->SetLocalMtx( mTargetNode->GetWorldMtx() );
//			mNode->Update();
			/*
			mNode->SetTranslation( mTargetNode->GetTranslation() );
			mNode->SetRotation( mTargetNode->GetRotation() );
			mNode->SetScale( mTargetNode->GetScale() );				
			*/

			break;

		case CONSTRAINT_TYPE_TRANSLATION:
			mNode->SetTranslation(mTargetNode->GetTranslation() + mConstraintShift);			
			break;

		case CONSTRAINT_TYPE_ROTATION:			
			mNode->SetRotation(mTargetNode->GetRotation());			
			break;

		case CONSTRAINT_TYPE_ROTATION_TRANSLATION:	
			mNode->SetTranslation(mTargetNode->GetTranslation() + mConstraintShift);			
			mNode->SetRotation(mTargetNode->GetRotation());			
			break;

		case CONSTRAINT_TYPE_ROTATION_TRANSLATION_XZ:	
			PVRTVec3 nodePos = mNode->GetTranslation();
			PVRTVec3 targetPos = mTargetNode->GetWorldTranslation() + mConstraintShift;
			nodePos.x = targetPos.x;
			nodePos.z = targetPos.z;
			mNode->SetTranslation(nodePos);			
			PVRTQUATERNION rot;
			float angle;
			PVRTVec3 axis;
			PVRTMatrixQuaternionToAxisAngle(mTargetNode->GetRotation(), axis, angle);
			angle -= PVRT_PI;
			PVRTMatrixQuaternionRotationAxis(rot, axis, angle);
			mNode->SetRotation(rot);			
			break;

	}
}


