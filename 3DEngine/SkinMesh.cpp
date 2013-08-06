#include "SkinMesh.h"
#include "CameraManager.h"
#include "ModelData.h"
#include "RenderLayerManager.h"
#include "Shaders/ShaderWithSkin.h"
#include "Shaders/ShaderWithLightAndSkin.h"

NEWDEL_IMPL(SkinMesh)
NEWDEL_IMPL(SkinTransforms)
SkinMesh::SkinMesh()
{
	mSkinTransforms = NULL;
	//	mpSkeleton = NULL;
}
SkinMesh::~SkinMesh()
{
	if (mSkinTransforms)
	{
		delete [] mSkinTransforms;
	}
}

void SkinMesh::AllocateSkinTransforms()
{
	mSkinTransforms = new SkinTransforms[ mIndices->mSubMeshCount ];
}

SkinTransforms * SkinMesh::GetSkinTransform(int index)
{
	return mSkinTransforms + index;
}
/*

void SkinMesh::SetSkeleton(Node * pSkeleton)
{
mpSkeleton = pSkeleton;
}
*/


void SkinMesh::Update()
{
	Node::Update();
	//mpSkeleton->Update();
}

void SkinMesh::Apply()
{
	Node::Apply();

	if (!mVisible)
		return;

	if (!mInFrustum)
		return;

	RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
	renderManager.AddMesh(this);
}

void SkinMesh::Render()
{
	//mpSkeleton->Apply();

	CameraManager * pCameraManager = CameraManager::GetCameraManager();
	Camera * currentCamera = pCameraManager->GetCurrentCamera(); 

	mAppearance.Apply();

	ShaderWithSkin * pShader = (ShaderWithSkin*)mAppearance.GetShader();

	mVertices->Apply(pShader);
	mIndices->Apply();

	//GLuint bindBoneCount = pShader->GetBindBoneCount();
	GLuint bindBoneMatrixArray = pShader->GetBindBoneMatrixArray();

	int offset = 0;

	for (int subMesh = 0; subMesh < mIndices->mSubMeshCount ; subMesh++)
	{
		SkinTransforms * pSkinTransform = mSkinTransforms + subMesh;
		static PVRTMat4 boneWorld[MAX_JOINTS_PER_FACE];

		for(int i= 0; i < pSkinTransform->mNumTransforms; i++)
		{
			int boneIndex = pSkinTransform->mTransformIds[i];
			Node * pBone = (Mesh*)mParentModel->mTransforms.GetArray(boneIndex);
			if (!pBone->IsInFrustum())
			{
				return;
			}
			PVRTMatrixMultiply(boneWorld[i], pBone->GetWorldInverseMtx(), pBone->GetWorldMtx() );
		}

		//ERR( glUniform1i(bindBoneCount, pSkinTransform->mNumTransforms) );
		ERR( glUniformMatrix4fv(bindBoneMatrixArray, pSkinTransform->mNumTransforms, GL_FALSE, boneWorld[0].f ) );			
		{
			//if (mIndices->mVbo)
				ERR( glDrawElements(GL_TRIANGLES, mIndices->mSubMeshSizes[subMesh], GL_UNSIGNED_SHORT, &((GLshort*)0)[offset]) );		
		}
		offset += mIndices->mSubMeshSizes[subMesh];
	}
}

