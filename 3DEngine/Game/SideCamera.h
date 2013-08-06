#ifndef __BackCamera__
#define __BackCamera__

#include "../Config.h"
#include "../Node.h"
#include "../Camera.h"
#include "../WiresData.h"

class BackCamera {
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

	void SetMaxDistance(float dist);
	float GetMaxDistance();

	void SetMinDistance(float dist);
	float GetMinDistance();

	void SetHeight(float height);
	float GetHeight();

	void Update();

	void SetBorderWire(Wire * pWire);


protected:
	float mMaxDist;
	float mHeight;

	Camera * mCamera;
	Node * mNode;

	Wire * mBorderWire;
};

#endif //__BackCamera__
