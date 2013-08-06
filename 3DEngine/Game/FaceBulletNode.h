#ifndef __FaceBulletNode__
#define __FaceBulletNode__

#include "../Config.h"
#include "BulletNode.h"



class FaceBulletNode : public BulletNode {
public:
	NEWDEL_DECL;
	
	FaceBulletNode();
	virtual ~FaceBulletNode();

	virtual void ShootState();
	virtual void FlyState();
	void SetMesh(Mesh * pMesh)
	{
		mMesh = pMesh;
	}

protected:
	Mesh * mMesh;
	int mImageCount;
	
};

#endif //__FaceBulletNode__
