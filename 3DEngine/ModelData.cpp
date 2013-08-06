#include "ModelData.h"

NEWDEL_IMPL(ModelData)
NEWDEL_IMPL(ModelAnimations)

ModelData::ModelData(void)
{
	mRoot = NULL;
	mAnimating = true;
}

ModelData::~ModelData(void)
{
	//Destroyed seperatly...
	//for (int n=0;n<mTextures.size();n++)
	//	delete mTextures[n];
	ListIterator matIter(mMaterials);
	while (MaterialData * pMatrerial = (MaterialData*)matIter.GetPtr() )
	{
		delete pMatrerial;
	}

	ListIterator transIter(mTransforms);
	while (Node * pTrans = (Node*)transIter.GetPtr() )
	{
		delete pTrans;
	}

	ListIterator meshIter(mMeshes);
	while (Mesh * pMeshs = (Mesh*)meshIter.GetPtr() )
	{
		delete pMeshs;
	}

	ListIterator cameraIter(mCameras);
	while (Camera * pCamera = (Camera*)cameraIter.GetPtr() )
	{
		delete pCamera;
	}
	delete mRoot;

}

void ModelData::SetLayer(int layer)
{
	ListIterator meshIter(mMeshes);
	while (Mesh * pMeshs = (Mesh*)meshIter.GetPtr() )
	{
		pMeshs->SetLayer(layer);
	}

}

