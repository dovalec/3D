#include "OverlayFace.h"
#include "RenderLayerManager.h"
#include "ResourceLoader.h"

NEWDEL_IMPL(OverlayFace)
OverlayFace::OverlayFace()
{
	static unsigned short indices[] = {0,1,2,3};

	static Vertex vertices[] = { -1,-1,0,		0,0,-1,		0,1, 
						1,-1,0,		0,0,-1,		1,1, 
						-1,1,0,		0,0,-1,		0,0,
						1,1,0,		0,0,-1,		1,0
					};


	mVertices = new Vertices();
	mIndices = new Indices();

	mVertices->Allocate(sizeof(Vertex), 4);
	mIndices->Allocate(4);

	mVertices->PutValues(vertices);
	mIndices->PutValues(indices);
	
	SetVertices(mVertices);
	SetIndices(mIndices);

	mIndices->mTriangleMode = TrianglesModeTrianglesStrip;

	mLayer = 3;

	mUseOcclusionCulling = false;
}


OverlayFace::~OverlayFace()
{
	ResourceLoader & resourceLoader = ResourceLoader::GetResourceLoader();
	resourceLoader.RemoveOverlayFace(this);
	
	if (mVertices)
		delete mVertices;

	if (mIndices)
		delete mIndices;
}


void OverlayFace::Apply()
{
	if (!mVisible)
		return;

	Node::Apply();

	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	renderManager.AddMesh(this);
}


void OverlayFace::Render()
{
	if (!mVisible)
		return;


	Shader * pShader = mAppearance.GetShader();
	pShader->SetOrthographic();

	mAppearance.mCompositingMode.mEnableDepth = false;
	mAppearance.Apply();
	
	mVertices->Apply(pShader);
	mIndices->Apply();

	ERR( glDrawElements(GL_TRIANGLE_STRIP, mIndices->mCount, GL_UNSIGNED_SHORT, 0) );
}

