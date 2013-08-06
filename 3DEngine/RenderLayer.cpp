#include "RenderLayer.h"
#include "RenderLayerManager.h"

NEWDEL_IMPL(RenderLayer)
RenderLayer::RenderLayer()
{
	mOpaqueCount = 0;
	mTransparentCount = 0;

}

RenderLayer::~RenderLayer()
{
	
}



void RenderLayer::Reset()
{
	mOpaqueCount = 0;
	mTransparentCount = 0;
}

void RenderLayer::SetupRenderStates()
{
	//ERR( glCullFace(GL_BACK) );
	//ERR( glEnable(GL_CULL_FACE) );
	// --- ERR( glEnable(GL_DEPTH_TEST) );

	//ERR( glDepthMask(true) );

	// --- ERR( glDepthFunc( GL_LEQUAL) );		
	//ERR( glDepthFunc( GL_ALWAYS) );

	//ERR( glEnable( GL_POLYGON_OFFSET_FILL ) );
	//ERR( glPolygonOffset(0.0f, 1.0f) );
		
}

int compareOpaque (const void * a, const void * b)
{
	Mesh * pMesh1 = *((Mesh**)(a));
	Mesh * pMesh2 = *((Mesh**)(b));

	if (pMesh1->GetAppearance().GetTexture() == pMesh2->GetAppearance().GetTexture())
		return 0;
	return -1;
}


int compareTransparent (const void * a, const void * b)
{
	Mesh * pMesh1 = *((Mesh**)(a));
	Mesh * pMesh2 = *((Mesh**)(b));

	if (pMesh1->GetAppearance().GetTexture() == pMesh2->GetAppearance().GetTexture())
		return 0;
	return -1;
}

void RenderLayer::SortOpaque()
{
	//qsort (mRenderListOpaque, mOpaqueCount, sizeof(void*), compareOpaque );
}

void RenderLayer::SortTransparent()
{
	//qsort (mRenderListTransparent, mTransparentCount, sizeof(void*), compareTransparent );
}

void RenderLayer::RenderOpaque()
{
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	bool centerCulling = renderManager.GetCenterCulling();
	const PVRTVec3 & center =  renderManager.GetCenter();
	float occlusionRadius = renderManager.GetOcclusionRadius();

	SortOpaque();
	
	for (int n=0;n<mOpaqueCount;n++)
	{
		Mesh * pMesh = mRenderListOpaque[n];
		renderManager.SetCurrentMesh(pMesh);

/*		if (centerCulling && pMesh->GetUseOcclusionCulling())
		{
			PVRTVec3 worldPos = pMesh->GetWorldTranslation();
			PVRTVec3 distVec = worldPos - center;
			if (  MM(pMesh->GetRadius() + occlusionRadius ) > distVec.lenSqr())
			{			
				pMesh->Render();
			}			
		}
		else
*/		{
			pMesh->Render();
		}
	}
}
void RenderLayer::RenderTransparent()
{
	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	bool centerCulling = renderManager.GetCenterCulling();
	const PVRTVec3 & center =  renderManager.GetCenter();
	float occlusionRadius = renderManager.GetOcclusionRadius();

	SortTransparent();

	for (int n=0;n<mTransparentCount;n++)
	{
		Mesh * pMesh = mRenderListTransparent[n];
		renderManager.SetCurrentMesh(pMesh);

		//if (!pMesh->GetUseBackFaceCulling())
		//{
			//ERR( glCullFace(GL_FRONT_AND_BACK ) );
			//ERR( glDisable(GL_CULL_FACE) );
		//}
/*
		if (centerCulling && pMesh->GetUseOcclusionCulling())
		{
			PVRTVec3 worldPos = pMesh->GetWorldTranslation();
			PVRTVec3 distVec = worldPos - center;
			if (  MM(pMesh->GetRadius() + occlusionRadius ) > distVec.lenSqr())
			{
				pMesh->Render();
			}			
		}
		else
		{
	*/
			pMesh->Render();
	//	}

		//if (!pMesh->GetUseBackFaceCulling())
		//{
			//ERR( glEnable(GL_CULL_FACE) );
			//ERR( glCullFace(GL_BACK ) );
		//}
		
	}
}
