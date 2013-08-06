#include "RenderLayerManager.h"

NEWDEL_IMPL(RenderLayerManager)
RenderLayerManager::RenderLayerManager()
{
	mNumLayers = 0;
	mCenterCulling = false;
	mOcclusionRadius = GLOBAL_SCALE*100000.0f;
	mCurrentMesh = NULL;
	mCenterNode = NULL;
}

RenderLayerManager::~RenderLayerManager()
{
	delete [] mRenderLayers;
}

void RenderLayerManager::AllocateLayers(int numLayers)
{
	mRenderLayers = new RenderLayer[numLayers];
	mNumLayers = numLayers;
}
 

void RenderLayerManager::ClearScreen()
{
	ERR( glClearColor(0,0,0, 1.0f) );
	ERR( glClearDepthf(0.0f) );
	ERR( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	//ERR( glClear( GL_DEPTH_BUFFER_BIT ) );
}


void RenderLayerManager::RenderAll()
{
	ClearScreen();

	ERR( glEnable(GL_CULL_FACE) );
	ERR( glCullFace( GL_BACK) );
	ERR( glEnable( GL_DEPTH_TEST) );
	ERR( glDepthFunc(GL_GEQUAL) );


	for (int n=0;n < mNumLayers ; n++)
	{
		ERR( glEnable( GL_DEPTH_TEST) );
		ERR( glClear( GL_DEPTH_BUFFER_BIT) );
		ERR( glEnable( GL_CULL_FACE) );

		RenderLayer & layer = mRenderLayers[n];
		layer.RenderOpaque();

		ERR( glDisable(GL_CULL_FACE) );
		if (n>2)
		{
			ERR( glDisable( GL_DEPTH_TEST) );
		}
		layer.RenderTransparent();
		layer.Reset();
	}

	//ERR( glDisable( GL_DEPTH_TEST) );
	//ERR( glDepthMask(true) );
	

	for (int n=0;n < mNumLayers ; n++)
	{
		RenderLayer & layer = mRenderLayers[n];

		
	}
	
}


