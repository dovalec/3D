#include "AnimationManager.h"
#include "../Shell.h"

NEWDEL_IMPL(AnimationManager)
AnimationManager::AnimationManager(void)
{
}

AnimationManager::~AnimationManager(void)
{

}

AnimationClip * AnimationManager::LoadClip(const char * clipName)
{

	AnimationClip * pClip = (AnimationClip*)mClipMap.GetByKey(clipName);

	if ( pClip )
		return pClip;

	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char modelClipName[256]={0};
	sprintf(modelClipName,"%s%s", folder, clipName);

	//LOGI("Loading clip: %s", modelClipName);

	pClip = new AnimationClip();

	Loader loader;
	if (!loader.OpenFile(modelClipName))
	{
		delete pClip;
		return NULL;
	}
	if (!ReadClip(pClip, loader))
	{
		LOGE("Error reading clip");
		delete pClip;
		return NULL;
	}

	loader.FreeData();
	mClipMap.Add( clipName, pClip);
	pClip->mName = clipName;
	return pClip;
}

AnimationUVClip * AnimationManager::LoadUVClip(const char * clipName)
{
	AnimationUVClip * pClip = (AnimationUVClip*)mClipMap.GetByKey(clipName);
	if (pClip)
		return pClip;
	
	Shell * pShell = Shell::GetShell();
	const char * folder = pShell->GetReadPath();

	static char uvClipName[256]={0};
	sprintf(uvClipName,"%s%s", folder, clipName);

	LOGI("Loading uv clip: %s", uvClipName);

	pClip = new AnimationUVClip();

	Loader loader;
	if (!loader.OpenFile(uvClipName))
	{
		delete pClip;
		return NULL;
	}
	if (!ReadUVClip(pClip, loader))
	{
		LOGE("Error reading uv clip");
		delete pClip;
		return NULL;
	}

	loader.FreeData();
	mClipMap.Add( clipName, pClip);
	return pClip;
}


bool AnimationManager::ReadClip(AnimationClip * pClip, Loader & loader)
{
	int transformCount = loader.ReadInt(); 
	float duration = loader.ReadFloat();

	pClip->SetDuration(duration);
	pClip->AllocateTransformKeys(transformCount);

	for (int n=0;n<transformCount;n++)
	{
		int transformIndex = loader.ReadInt();
	
		int numOfRotationKey = loader.ReadInt();
		int numOfTranslateKey = loader.ReadInt();
		int	numOfScaleKey = loader.ReadInt();
		int	numOfVisibilityKey = loader.ReadInt();

		TransformAndKeys & transAndKeys = pClip->GetTransformAndKeysByNum(n);
		transAndKeys.transformIndex = transformIndex;
	
		if (numOfRotationKey)
		{
			pClip->SetNumKeys(numOfRotationKey);
			transAndKeys.keys.SetNumKeys(numOfRotationKey);
			transAndKeys.keys.AllocateRotation();
			transAndKeys.keys.PutRotateKeys( (RotateKey*) loader.GetReaderPtr() );
			loader.Progress(sizeof(RotateKey) * numOfRotationKey);

		}
		
		if (numOfTranslateKey)
		{
			pClip->SetNumKeys(numOfTranslateKey);
			transAndKeys.keys.SetNumKeys(numOfTranslateKey);
			transAndKeys.keys.AllocateTranslation();
			transAndKeys.keys.PutTranslateKeys( (TranslateKey*) loader.GetReaderPtr() );
			loader.Progress(sizeof(TranslateKey) * numOfTranslateKey);
		}
		
		if (numOfScaleKey)
		{
			pClip->SetNumKeys(numOfScaleKey);
			transAndKeys.keys.SetNumKeys(numOfScaleKey);
			transAndKeys.keys.AllocateScale();
			transAndKeys.keys.PutScaleKeys( (ScaleKey*) loader.GetReaderPtr() );
			loader.Progress(sizeof(ScaleKey) * numOfScaleKey);
		}

		if (numOfVisibilityKey)
		{
			pClip->SetNumKeys(numOfVisibilityKey);
			transAndKeys.keys.SetNumKeys(numOfVisibilityKey);
			transAndKeys.keys.AllocateVisibility();
			transAndKeys.keys.PutVisibilityKeys( (int*) loader.GetReaderPtr() );
			loader.Progress(sizeof(int) * numOfVisibilityKey);
		}
	}


	return true;
}


bool AnimationManager::ReadUVClip(AnimationUVClip * pClip, Loader & loader)
{
	int meshIndex = loader.ReadInt(); 
	int numVertices = loader.ReadInt(); 
	//float duration = loader.ReadFloat();
	//pClip->SetDuration(duration);

	pClip->SetMeshIndex(meshIndex);
	pClip->AllocateVertexUVKeys(numVertices);

	for (int n=0;n<numVertices;n++)
	{
		int vertex = loader.ReadInt();
		int numKeys = loader.ReadInt();
		
		pClip->SetNumKeys(numKeys);

		VertexUVKeys & vertexUVKeys = pClip->GetVertexUVKeysByNum(n);
		vertexUVKeys.vertex = vertex;
		vertexUVKeys.keys.SetNumKeys( numKeys );
		vertexUVKeys.keys.AllocateKeys();
		vertexUVKeys.keys.PutKeys( (UVKey*) loader.GetReaderPtr() );
		loader.Progress( sizeof(UVKey) * numKeys );

	}

	return true;
}


void AnimationManager::Destroy()
{
	MapIterator mapIter(mClipMap);
	while (AnimationClip * pClip = (AnimationClip*)mapIter.GetPtr())
	{
		delete pClip;
	}

	mClipMap.RemoveAll();
}

void AnimationManager::DestroyClip(const char * clipName)
{
	AnimationClip * pClip = (AnimationClip*)mClipMap.GetByKey( clipName );
	if (pClip == NULL)
	{
		STOP;
	}

	mClipMap.RemoveByValue(pClip);

	delete(pClip);
}
