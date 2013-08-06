 #include "Camera.h"
#include "Shell.h"

NEWDEL_IMPL(Camera)
Camera::Camera()
{
	mWidth = 0;
	mHeight = 0;
	
	mVerticalFieldOfView = PVRT_PI / 5;
	mNearClippingPlane = GLOBAL_SCALE*1.0f;
	mFarClippingPlane = GLOBAL_SCALE*300.0f;
}

void Camera::Update() {
	
	UpdateInverseWorldTransform();
	Node::Update();

	Shell * pShell = Shell::GetShell();
	int width;
	int height;
	pShell->GetScreenSize(width, height);

	if (width != mWidth || height != mHeight)
	{
		mAspectRatio = (float)width / (float)height;
		
		
		mProjection = PVRTMat4::PerspectiveFovFloatDepthRH(mVerticalFieldOfView, mAspectRatio, mNearClippingPlane, PVRTMat4::OGL, false);
		//mProjection = PVRTMat4::PerspectiveFovRH(mVerticalFieldOfView, mAspectRatio, mNearClippingPlane, mFarClippingPlane, PVRTMat4::OGL, false);
		mWidth = width;
		mHeight = height;
	}

	PVRTVec3 position = GetWorldTranslation();
	mView =  mWorldInverse;
	PVRTMatrixMultiply(mViewProjection, mWorldInverse, mProjection);

}
