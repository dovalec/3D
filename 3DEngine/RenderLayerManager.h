#ifndef __RenderLayerManager__
#define __RenderLayerManager__

#include "Config.h"
#include "Mesh.h"
#include "RenderLayer.h"

#define MAX_RENDER_LAYERS 32

class RenderLayerManager {
public:
	NEWDEL_DECL
	RenderLayerManager();
	~RenderLayerManager();
	
	inline static RenderLayerManager & GetRenderLayerManager()
	{
		return *GlobalSys::GetRenderLayerManager();		
	}

	void AllocateLayers(int numLayers);

	inline void AddMesh(Mesh * pMesh)
	{		
		int layer = pMesh->GetLayer();
		mRenderLayers[layer].AddMesh(pMesh);
	}

	void ClearScreen();
	void RenderAll();
	
	void SetCenterCulling(bool cull)
	{
		mCenterCulling = cull;
	}

	inline bool GetCenterCulling()
	{
		return mCenterCulling;
	}

	inline const PVRTVec3 GetCenter()
	{
		if (mCenterNode)
		{
			return mCenterNode->GetWorldTranslation();
		}
		return mCenter;
	}

	inline void SetCenter(PVRTVec3 center)
	{
		mCenter = center;
	}

	inline void SetCenterNode(Node * centerNode)
	{
		mCenterNode = centerNode;
	}



	inline float GetOcclusionRadius()
	{
		return mOcclusionRadius;
	}

	inline void SetOcclusionRadius(float radius)
	{
		mOcclusionRadius = radius;
	}

	inline Mesh * GetCurrentMesh()
	{
		return mCurrentMesh;
	}

	inline void SetCurrentMesh(Mesh * pMesh)
	{
		mCurrentMesh = pMesh;
	}

	inline RenderLayer & GetRenderLayer(int index)
	{
		if (index >= mNumLayers)
		{
			LOGE("index >= mNumLayers");
			STOP;
		}
		return mRenderLayers[index];
	}

	void Destroy()
	{
		if (mRenderLayers)
			delete [] mRenderLayers;
	}

protected:
	RenderLayer * mRenderLayers;
	Node * mCenterNode;
	int mNumLayers;
	bool mCenterCulling;
	PVRTVec3 mCenter;
	float mOcclusionRadius;
	Mesh * mCurrentMesh;

	
};	

#endif //__RenderLayerManager__
