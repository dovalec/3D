#ifndef __RenderLayer__
#define __RenderLayer__

#include "Config.h"
#include "Mesh.h"

#define MAX_RENDER_LIST_OPAQUE 2048
#define MAX_RENDER_LIST_TRANS 2048


class RenderLayer {
public:
	NEWDEL_DECL
	RenderLayer();
	virtual ~RenderLayer();


	inline void AddMesh(Mesh * pMesh)
	{	

		if (pMesh->GetAppearance().mCompositingMode.mBlendingMode != BlendingModeReplace)
		{
			AddTransparent(pMesh);
		}
		else
		{
			AddOpaque(pMesh);
		}
	}

	void SortOpaque();
	void SortTransparent();

	inline void AddOpaque(Mesh * pMesh)
	{
		mRenderListOpaque[mOpaqueCount] = pMesh;
		mOpaqueCount++;
		if (mOpaqueCount == MAX_RENDER_LIST_OPAQUE)
		{
			LOGE("mOpaqueCount == MAX_RENDER_LIST_OPAQUE");
			STOP;
		}

	}

	inline void AddTransparent(Mesh * pMesh)
	{
		mRenderListTransparent[mTransparentCount] = pMesh;
		mTransparentCount++;
		if (mTransparentCount == MAX_RENDER_LIST_TRANS)
		{
			LOGE("mTransparentCount == MAX_RENDER_LIST_TRANS");
			STOP;
		}
	}

	void SetupRenderStates();

	void RenderOpaque();
	void RenderTransparent();

	void Reset();

protected:
	Mesh * mRenderListOpaque[MAX_RENDER_LIST_OPAQUE];
	Mesh * mRenderListTransparent[MAX_RENDER_LIST_TRANS];

	int mOpaqueCount;
	int mTransparentCount;

};	

#endif //__RenderLayer__
