#include "FaceBulletNode.h"
#include "../TLSF/Tlsf.h"

NEWDEL_IMPL(FaceBulletNode)

FaceBulletNode::FaceBulletNode()
{
	
	mImageCount = 0;
}

FaceBulletNode::~FaceBulletNode()
{

}

void FaceBulletNode::ShootState()
{
	BulletNode::ShootState();
	mImageCount = 0;

}

void FaceBulletNode::FlyState()
{
	BulletNode::FlyState();

	if (mImageCount<25)
	{
		PVRTMat4 & uvMtx = mMesh->GetAppearance().GetUVMatrix();
		PVRTMatrixTranslation(uvMtx, 0.2f * (float)(mImageCount%5), 0.2f * (float)(mImageCount/5), 0);
		mImageCount++;
	}
}


