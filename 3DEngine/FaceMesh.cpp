#include "FaceMesh.h"
#include "Shell.h"
#include "RenderLayerManager.h"

unsigned short gIndices[] = {0,1,2,3};

Vertex gVertices[] = { -1,-1,0,		0,0,-1,		0,1, 
						1,-1,0,		0,0,-1,		1,1, 
						-1,1,0,		0,0,-1,		0,0,
						1,1,0,		0,0,-1,		1,0
					};

NEWDEL_IMPL(FaceMesh)
FaceMesh::FaceMesh()
{
	
	mVertices = new Vertices();
	mIndices = new Indices();

	mVertices->Allocate(sizeof(Vertex), 4);
	mIndices->Allocate(4);

	mVertices->PutValues(gVertices);
	mIndices->PutValues(gIndices);

	SetVertices(mVertices);
	SetIndices(mIndices);

	
	mIndices->mTriangleMode = TrianglesModeTrianglesStrip;
}

void FaceMesh::Apply()
{
	Node::Apply();

	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	renderManager.AddMesh(this);
}


void FaceMesh::Render()
{	
	Shell * pShell = Shell::GetShell();
	int w,h;
	pShell->GetScreenSize(w,h);

	mAppearance.mCompositingMode.mEnableDepth = false;
	mAppearance.Apply();
	

	Shader * pShader = mAppearance.GetShader();

	pShader->SetOrthographic();

	mVertices->Apply(pShader);
	mIndices->Apply();

	ERR( glDrawElements(GL_TRIANGLE_STRIP, mIndices->mCount, GL_UNSIGNED_SHORT, 0) );
	
}


