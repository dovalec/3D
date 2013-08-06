#include "CameraLookAt.h"
#include "Shell.h"

NEWDEL_IMPL(CameraLookAt)
CameraLookAt::CameraLookAt()
{	
	mUp = PVRTVec3(0.f, 1.f, 0.f);
	mTarget = PVRTVec3(0,0,0);
}

void CameraLookAt::Update() {
	
	Camera::Update();

	PVRTVec3 position = GetWorldTranslation();
	mView =  PVRTMat4::LookAtRH(position, mTarget, mUp);
	PVRTMatrixMultiply(mViewProjection, mView, mProjection);

}
