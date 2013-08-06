#ifndef __CameraLookAt__
#define __CameraLookAt__

#include "Config.h"
#include "Camera.h"

class CameraLookAt : public Camera {

public:
	NEWDEL_DECL
	CameraLookAt();

	virtual void Update();

	inline PVRTVec3 & GetTarget()
	{
		return mTarget;
	}

	inline void SetTarget( PVRTVec3 target)
	{
		mTarget = target;
	}

	inline void SetTarget( float x, float y, float z)
	{
		mTarget.x = x;
		mTarget.y = y;
		mTarget.z = z;
	}

	inline PVRTVec3 & GetUp()
	{
		return mUp;
	}


protected:
	PVRTVec3 mTarget;
	PVRTVec3 mUp;

};

#endif //__CameraLookAt__
