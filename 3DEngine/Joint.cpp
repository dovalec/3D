#include "Node.h"
#include "RenderLayerManager.h"

#include "Camera.h"
#include "CameraManager.h"
NEWDEL_IMPL(Node)
Node::Node()
{
	mParentIndex = -1;
	mParentModel = NULL;

	mVisible = true;
	mUseOcclusionCulling = true;
	mInFrustum = true;
	mPicked = false;
	mParent = NULL;

	UpdateMatrices();
	UpdateInverseWorldTransform();

	PVRTMatrixIdentity(mWorld);
	PVRTMatrixIdentity(mLocal);
	PVRTMatrixIdentity(mWorldInverse);

	mTranslation = PVRTVec3(0,0,0);
	mPivot = PVRTVec3(0,0,0);

	PVRTMatrixQuaternionIdentity(mRotation);
	mScale = PVRTVec3(1,1,1);


}

void Node::SetName(String name)
{
	mName = name;
}

const String & Node::GetName()
{
	return mName;
}

void Node::SetVisible(bool flag)
{
	mVisible = flag;
}

void Node::SetVisisbleChildren(bool flag)
{
	mVisible = flag;

	ListIterator iter(mChildren);
	while (Node * pChild = (Node*)iter.GetPtr())
	{
		pChild->SetVisisbleChildren(flag);
	}

}

void Node::SetPicked(bool flag)
{
	mPicked = flag;
}


bool Node::GetPicked()
{
	return mPicked;
}

void Node::AddChild(Node * child)
{
	assert(!mChildren.Find(child));
	
	if (child->mParent != NULL)
		child->mParent->RemoveChild(child);

	child->mParent = this;
	mChildren.Add(child);


}

void Node::RemoveChild(Node * child)
{
	mChildren.Remove(child);
	
	child->mParent = NULL;
	child->mParentIndex = -1;
}


void Node::Render()
{
	ListIterator iter(mChildren);
	while (Node * pChild = (Node*)iter.GetPtr())
	{
		pChild->Render();
	}
}

void Node::Update()
{
	if (mUpdateLocal)
	{
		UpdateLocalMatrix();
		mUpdateLocal = false;
	}

	UpdateWorldTransform();

	if (!mVisible)
		return;

	if ( mParent && !mParent->IsInFrustum() )
	{
		mInFrustum = false;
		return;
	}

	if ( mUseOcclusionCulling)
	{
		RenderLayerManager & renderManager = RenderLayerManager::GetRenderLayerManager();
		const PVRTVec3 center =  renderManager.GetCenter();
		float occlusionRadius = renderManager.GetOcclusionRadius();

		PVRTVec3 worldPos = GetWorldTranslation();
		PVRTVec3 distVec = worldPos - center;

		if ( distVec.lenSqr() < MM(occlusionRadius) ) 
		{
			mInFrustum = true;
		}
		else
		{
			mInFrustum = false;
			return;
		}
	}
	else
	{
		mInFrustum = true;
	}

	ListIterator iter(mChildren);
	while (Node * pChild = (Node*)iter.GetPtr())
	{
		pChild->Update();
	}

}

PVRTVec2 Node::GetProjected()
{
	PVRTVec3 pos(mWorld.f[12], mWorld.f[13], mWorld.f[14]);
	CameraManager & cameraManager = CameraManager::GetCameraManager();
	Camera * pCamera = cameraManager.GetCurrentCamera();
	PVRTMat4 projMtx(pCamera->GetViewProjMtx().f);
	PVRTVec4 vec4(pos.x, pos.y , pos.z, 1.0f);
	PVRTVec4 screenPos4 = projMtx * vec4;
    const float rhw = 1.0f / screenPos4.w;
    screenPos4.x = screenPos4.x * rhw;
	screenPos4.y = screenPos4.y * rhw;
	PVRTVec2 screenPos2(screenPos4.x, screenPos4.y);
	return screenPos2;
}
void Node::UpdateLocalMatrix()
{
	PVRTMATRIX transMtx;
	PVRTMatrixTranslation(transMtx, mTranslation.x, mTranslation.y, mTranslation.z);

	PVRTMATRIX pivotMtx;
	PVRTMatrixTranslation(pivotMtx, mPivot.x, mPivot.y, mPivot.z);

	PVRTMATRIX pivotNegateMtx;
	PVRTMatrixTranslation(pivotNegateMtx, -mPivot.x, -mPivot.y, -mPivot.z);

	PVRTQUATERNION quat;
	PVRTMATRIX rotMtx;
	quat.x = mRotation.x;
	quat.y = mRotation.y;
	quat.z = mRotation.z;
	quat.w = mRotation.w;
	PVRTMatrixRotationQuaternion(rotMtx, quat);

	PVRTMATRIX scaleMtx;
	PVRTMatrixScaling(scaleMtx, mScale.x, mScale.y , mScale.z);


	//transformMatrix = TranslationMatrix( -pivotPoint ) * RotationMatrix( rotation ) * Translationmatrix( pivotPoint ) * TranslationMatrix( translation )

	//PVRTMATRIX tmpMtx;

	//PVRTMatrixIdentity(mLocal);
	//PVRTMatrixMultiply(mLocal, mLocal, scaleMtx);
	PVRTMatrixMultiply(mLocal, scaleMtx, pivotNegateMtx);
	PVRTMatrixMultiply(mLocal, mLocal, rotMtx);
	PVRTMatrixMultiply(mLocal, mLocal, pivotMtx);
	PVRTMatrixMultiply(mLocal, mLocal, transMtx);
/*
	PVRTMatrixMultiply(mLocal, scaleMtx, rotMtx);
	PVRTMatrixMultiply(mLocal, mLocal, pivotNegateMtx);
	PVRTMatrixMultiply(mLocal, mLocal, pivotMtx);
	PVRTMatrixMultiply(mLocal, mLocal, pivotMtx);
	PVRTMatrixMultiply(mLocal, mLocal, transMtx);
*/
}

void Node::UpdateWorldTransform()
{
	if (mParent == NULL)
	{
		mWorld = mLocal;
	}
	else
	{
		PVRTMatrixMultiply(mWorld, mLocal ,mParent->mWorld);
	}

	if (mUpdateInverse)
	{
		PVRTMatrixInverse(mWorldInverse, mWorld);
		mUpdateInverse = false;
	}

	
}

PVRTMat4 Node::LookAt(const PVRTVec3& vEye, const PVRTVec3& vAt, const PVRTVec3& vUp, bool bRightHanded)
{
		PVRTVec3 vForward, vUpNorm, vSide;
		PVRTMat4 result;

		vForward = (bRightHanded) ? vEye - vAt : vAt - vEye;

		vForward.normalize();
		vUpNorm = vUp.normalized();
		vSide   = vUpNorm.cross( vForward);
		vUpNorm = vForward.cross(vSide);

		result.f[0]=-vSide.x;		result.f[4]=vSide.y;		result.f[8]=-vSide.z;			result.f[12]=0;
		result.f[1]=vUpNorm.x;		result.f[5]=vUpNorm.y;		result.f[9]=vUpNorm.z;			result.f[13]=0;
		result.f[2]=vForward.x;		result.f[6]=vForward.y;		result.f[10]=vForward.z;		result.f[14]=0;
		result.f[3]=0;				result.f[7]=0;				result.f[11]=0;					result.f[15]=f2vt(1);


		result.postTranslate(-vEye);
		return result;
}


void Node::printWorld()
{
#ifdef WIN32
	if (mName.GetPtr())
		return;

	static char txt[256]={0};
	sprintf(txt,"%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n",	mWorld.f[0],
																							mWorld.f[1],
																							mWorld.f[2],
																							mWorld.f[3],
																							mWorld.f[4],
																							mWorld.f[5],
																							mWorld.f[6],
																							mWorld.f[7],
																							mWorld.f[8],
																							mWorld.f[9],
																							mWorld.f[10],
																							mWorld.f[11],
																							mWorld.f[12],
																							mWorld.f[13],
																							mWorld.f[14],
																							mWorld.f[15]);

	OutputDebugString(mName.GetPtr());
	OutputDebugString("\n");
	OutputDebugString(txt);
	OutputDebugString("\n");

#endif
}

void Node::UpdateInverseWorldTransform()
{
	mUpdateInverse = true;
}

void Node::Apply()
{
	if (!mVisible)
		return;

	if (!mInFrustum)
		return;

	ListIterator iter(mChildren);
	while (Node * pChild = (Node*)iter.GetPtr())
	{
		pChild->Apply();
	}

}

void Node::SetUseOcclusionCullingTraverse(bool culling)
{
	mUseOcclusionCulling = culling;
	ListIterator iter(mChildren);
	while (Node * pChild = (Node*)iter.GetPtr())
	{
		pChild->SetUseOcclusionCullingTraverse(culling);
	}
}


