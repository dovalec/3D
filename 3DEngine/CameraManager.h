#ifndef __CameraManager__
#define __CameraManager__

#include "Config.h"
#include "Camera.h"

class CameraManager {
public:
	CameraManager();
	NEWDEL_DECL
	inline static CameraManager * GetCameraManager()
	{
		return GlobalHeap::GetCameraManager();
	}

	void Destroy()
	{

	}

	void SetCurrentCamera(Camera * pCamera);
	inline Camera * GetCurrentCamera()
	{
		return mCurrentCamera;
	}


protected:
	Camera * mCurrentCamera;
};

#endif //__CameraManager__
