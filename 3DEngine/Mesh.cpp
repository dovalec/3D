#include "Mesh.h"
#include "CameraManager.h"
#include "RenderLayerManager.h"
#include "Shaders/ShaderWithLight.h"
#include "Shaders/ShaderWithTwoLights.h"

NEWDEL_IMPL(Mesh)
Mesh::Mesh()
{
	mNumOfAppearances = 0;
	mVertices = NULL;
	mIndices = NULL;
	mLayer = 0;

	mUseDepth = true;
	mUseBackFaceCulling = true;

}


void Mesh::SetLayer(int layer)
{
	mLayer = layer;
}

void Mesh::SetDisplayMode(DisplayMode mode)
{
	mDisplayMode = mode;
}


void Mesh::Render()
{
	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * currentCamera = pCameraManager->GetCurrentCamera(); 
	
	mAppearance.Apply();
	ShaderWithTwoLights * pShader = (ShaderWithTwoLights*)mAppearance.GetShader();
	mVertices->Apply(pShader);
	mIndices->Apply();

	int offset = 0;


	for (int subMesh = 0; subMesh < mIndices->mSubMeshCount ; subMesh++)
	{

		//if (mIndices->mVbo)
			ERR( glDrawElements(GL_TRIANGLES, mIndices->mSubMeshSizes[subMesh], GL_UNSIGNED_SHORT, &((GLshort*)0)[offset]) );
		offset += mIndices->mSubMeshSizes[subMesh];
	}
}

void Mesh::Update()
{	/*
	if (mDisplayMode == DisplayModeBillBoard)
	{
		CameraManager * pCameraManager = CameraManager::GetCameraManager();
		Camera * pCurrentCamera = pCameraManager->GetCurrentCamera();
		if (pCurrentCamera)
		{
			PVRTVec3 camWorldPos = pCurrentCamera->GetWorldTranslation();
			PVRTVec3 camTarget = pCurrentCamera->GetTarget();
			PVRTVec3 dir = camWorldPos - camTarget;
			dir.normalize();

			float ang_y = atan2f(dir.x, dir.z);
			PVRTQUATERNION quatY;
			PVRTMatrixQuaternionRotationAxis(quatY, PVRTVec3(0,1,0),ang_y);

			SetRotation(quatY);
		}
	}
	*/

	Node::Update();
}

void Mesh::Apply()
{
	Node::Apply();

	if (!mVisible)
		return;

	if (!mInFrustum)
		return;

	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	renderManager.AddMesh(this);
}

void Mesh::SetTransparent(bool state)
{
	if (state)
		mAppearance.mCompositingMode.mBlendingMode = BlendingModeAlpha;
	else
		mAppearance.mCompositingMode.mBlendingMode = BlendingModeReplace;

}