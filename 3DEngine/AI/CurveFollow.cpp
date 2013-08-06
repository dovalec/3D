#include "CurveFollow.h"

NEWDEL_IMPL(CurveFollow)
CurveFollow::CurveFollow(void)
{
	Reset();
}

CurveFollow::~CurveFollow(void)
{
}


void CurveFollow::MoveNode(Curve * pCurve, Node * pNode)
{

	float animationPos = mCurrentPos;

	float firstKey = floor(animationPos);
	float nextKey = ceil(animationPos);

	float s = animationPos - firstKey;

	CurvePoint & v1 = pCurve->curvePoints[(int)firstKey];
	CurvePoint & v2 = pCurve->curvePoints[(int)nextKey];
	CurvePoint pos;

	PVRTVECTOR3 tangentVec;
	PVRTQUATERNION tangentQuat;

	pos.x = v1.x + s * (v2.x - v1.x);
	pos.y = v1.y + s * (v2.y - v1.y);
	pos.z = v1.z + s * (v2.z - v1.z);

	tangentVec.x = v1.tx + s * (v2.tx - v1.tx);
	tangentVec.y = v1.ty + s * (v2.ty - v1.ty);
	tangentVec.z = v1.tz + s * (v2.tz - v1.tz);


	float rotAngle = atan2f(tangentVec.z, tangentVec.x) + PVRT_PI + PVRT_PI*0.5f;

	PVRTMatrixQuaternionRotationAxis(tangentQuat, PVRTVec3(0,1,0),rotAngle);

	pNode->SetRotation(tangentQuat.x , tangentQuat.y, tangentQuat.z, tangentQuat.w);
	pNode->SetTranslation(pos.x,pos.y,pos.z);

	/*
	float firstKey = floor(mCurrentPos);
	float nextKey = ceil(mCurrentPos);

	PVRTMATRIX & worldMatrix = pNode->GetWorldMtx();
	PVRTVec3 worldPos;
	worldPos.x = worldMatrix.f[12];
	worldPos.y = worldMatrix.f[13];
	worldPos.z = worldMatrix.f[14];

	FolowCurve(pCurve, worldPos);

	float rotAngle = GetDirectionAngle();
	PVRTVec3 & directionVec = GetDirectionVector();
	directionVec*=0.02f;

	PVRTMATRIX newPlane1RotMtx;
	PVRTMatrixRotationY(newPlane1RotMtx, rotAngle);

	PVRTMATRIX newTransMtx;
	PVRTMatrixTranslation(  newTransMtx,
	worldPos.x + directionVec.x,
	worldPos.y + directionVec.y,
	worldPos.z + directionVec.z );

	PVRTMatrixMultiply( pNode->GetLocalMtx(), newPlane1RotMtx, newTransMtx);
	*/
}

void CurveFollow::Reset()
{
	mWorldPos.x = 0.0f;
	mWorldPos.y = 0.0f;
	mWorldPos.z = 0.0f;

	mIndexOnCurve = 0;
	mNodeAtEnd = false;
	mAngle = 0.0f;
	mCurrentPos = 0;

}