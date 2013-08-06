#ifndef __SceneManager__
#define __SceneManager__

#include "Config.h"
#include "Node.h"
#include "QuadTree.h"
#include "TL/Map.h"

#define MAX_ROOT_NODES 2048

class SceneManager {
public:
	SceneManager();
	virtual ~SceneManager();

public:
	NEWDEL_DECL
	inline static SceneManager & GetSceneManager()
	{
		return *GlobalHeap::GetSceneManager();		
	}

	void Destroy()
	{

	}
	
	void Reset()
	{
		mNodeCount = 0;
	}
    
    void SetLookMtx(PVRTMat4 * pLookMtx)
    {
        mLookMtx = pLookMtx;
    }

	void SetSceneQuadTree(QuadTree * pQuadTree);

	void AddRootNode(Node * pNode);

	void Update();
	void Apply();

protected:
	Node * mToApply[MAX_ROOT_NODES];
	Node * mRootNodes[MAX_ROOT_NODES];

	PVRTMat4 * mLookMtx;

	int mToApplyCount;
	int mNodeCount;
	QuadTree * mQuadTree;

};

#endif //__SceneManager__
