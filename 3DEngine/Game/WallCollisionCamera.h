#ifndef __BackCamera__
#define __BackCamera__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"
#include "StopWireUser.h"

class BackCamera : public StopWireUser {
public:
	NEWDEL_DECL
	BackCamera();
	virtual ~BackCamera();
	
	inline Camera * GetCamera()
	{
		return mCamera;
	}

	void SetNode(Node * pNode);

	inline Node * GetNode()
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

	void SetHeight(float height);
	float GetHeight();

	void Update();

protected:
	float mMaxDist;
	float mHeight;
	float mFront;

	Camera * mCamera;
	Node * mNode;

};

#endif //__BackCamera__
