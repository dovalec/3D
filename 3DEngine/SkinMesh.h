#ifndef __SkinMesh__
#define __SkinMesh__


#include "Config.h"
#include "Mesh.h"
#include "VerticesSkin.h"

#define MAX_JOINTS_PER_FACE 12

class SkinTransforms {
public:
	NEWDEL_DECL
	SkinTransforms()
	{
		mNumTransforms = 0;
		memset(mTransformIds, 0, sizeof(mTransformIds));
	}

	int mNumTransforms;
	int mTransformIds[MAX_JOINTS_PER_FACE];
};

class SkinMesh : public Mesh
{
public:
	NEWDEL_DECL
	SkinMesh();
	virtual ~SkinMesh();

	void AllocateSkinTransforms();
	SkinTransforms * GetSkinTransform(int index);

	//void SetSkeleton(Node * pSkeleton);
	virtual void Render();
	virtual void Update();
	virtual void Apply();

	SkinTransforms * mSkinTransforms;

	//Node * mpSkeleton;
	//int mSubMeshCount;

};

#endif //__SkinMesh__
