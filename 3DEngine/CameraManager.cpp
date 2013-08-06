#include "CameraManager.h"

NEWDEL_IMPL(CameraManager)
CameraManager::CameraManager()
{
	mCurrentCamera = NULL;
}

void CameraManager::SetCurrentCamera(Camera * pCamera)
{
	mCurrentCamera = pCamera;
}


