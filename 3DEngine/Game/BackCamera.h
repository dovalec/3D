#ifndef __BackCamera__
#define __BackCamera__

#include "../Config.h"
#include "../Node.h"
#include "../CameraLookAt.h"
#include "../WiresData.h"

class BackCamera {
public:
	NEWDEL_DECL
	BackCamera();
	virtual ~BackCamera();
	
	inline CameraLookAt * GetCamera()
	{
		return mCamera;
	}

	void SetNode(Node * pNode);
    
    Node * GetNode()
    {
        return mNode;
    }

	void SetFront(bool state)
	{
		if (state)
			mFront = 1.0f;
		else
			mFront = -1.0f;
	}

	void SetMaxDistance(float dist);
	float GetMaxDistance();

	void SetMinDistance(float dist);
	float GetMinDistance();

	void SetLookAtHeight(float height);
	float GetLookAtHeight();

	void SetHeight(float height);
	float GetHeight();

	void OnTick();

protected:
    Node * mNode;
	float mMaxDist;
	float mHeight;
	float mLookAtHeight;
	float mFront;

	CameraLookAt * mCamera;

};

#endif //__BackCamera__
