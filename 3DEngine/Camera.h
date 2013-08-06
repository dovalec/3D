#ifndef __Camera__
#define __Camera__

#include "Config.h"
#include "Node.h"
#include "Vector3.h"

class Camera : public Node {

public:
	NEWDEL_DECL
	Camera();


	virtual void Update();

	inline PVRTMATRIX & GetViewProjMtx()
	{
		return mViewProjection;
	}

	inline PVRTMat4 & GetViewMtx()
	{
		return mView;
	}

	void SetAspectRatio(float v)
	{
		mAspectRatio = v;
	}
	void SetNearClippingPlane(float v)
	{
		mNearClippingPlane = v;
	}
	void SetFarClippingPlane(float v)
	{
		mFarClippingPlane = v;
	}
	void SetHorizontalFieldOfView(float v)
	{
		mHorizontalFieldOfView = v;
	}
	void SetVerticalFieldOfView(float v)
	{
		mVerticalFieldOfView = v;
	}
	inline float GetAspectRatio()
	{
		return mAspectRatio;
	}
	inline float GetNearClippingPlane()
	{
		return mNearClippingPlane;
	}
	inline float GetFarClippingPlane()
	{
		return mFarClippingPlane;
	}
	inline float GetHorizontalFieldOfView()
	{
		return mHorizontalFieldOfView;
	}
	inline float GetVerticalFieldOfView()
	{
		return mVerticalFieldOfView;
	}
protected:
	PVRTMat4 mView;
	PVRTMat4 mProjection;
	PVRTMat4 mViewProjection;

	int mWidth;
	int mHeight;
	float mAspectRatio;
	float mNearClippingPlane;
	float mFarClippingPlane;
	float mHorizontalFieldOfView;
	float mVerticalFieldOfView;
};

#endif //__Camera__
