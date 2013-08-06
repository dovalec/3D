#ifndef __ModelData__
#define __ModelData__

#include "Config.h"

#include "PVRTexture.h"
#include "MaterialData.h"
#include "Mesh.h"
#include "Camera.h"
#include "Node.h"
#include "Animation/AnimationClip.h"
#include "Animation/AnimationClipLogic.h"
#include "TL/List.h"
#include "TL/Map.h"

struct ModelAnimations
{
public:
	NEWDEL_DECL
	ModelAnimations()
	{		
	}

	inline void AddClip(AnimationClipLogic * pClipLogic, AnimTrackEnum track)
	{
		mClipsToPlay[track].Add(pClipLogic);
	}

	inline int RemoveCurrentClip(AnimTrackEnum track)
	{
		if (IsEmpty(track))
			return 0;

		mClipsToPlay[track].RemoveFirst();
		return mClipsToPlay[track].Size();
	}

	inline void ClearTrack(AnimTrackEnum track)
	{		
		mClipsToPlay[track].RemoveAll();		
	}

	inline bool IsEmpty(AnimTrackEnum track)
	{
		return mClipsToPlay[track].Size() == 0;
	}

	inline AnimationClipLogic * GetCurrentClip(AnimTrackEnum track)
	{
		if (IsEmpty(track))
			return NULL;
		
		return (AnimationClipLogic*)mClipsToPlay[track].First();
	}

private:
	List mClipsToPlay[(int)ANIM_TRACK_LAST];

};

class ModelData
{
public:
	NEWDEL_DECL
	ModelData(void);
	virtual ~ModelData(void);

	inline bool IsUsingAnimation()
	{
		return mAnimating;
	}

	inline void UseAnimation(bool state)
	{
		mAnimating = state;
	}


public:
	List mTextures;
	List mMaterials;
	List mTransforms;
	List mMeshes;
	List mCameras;

public:

	void SetFileName(String fileName)
	{
		mFileName = fileName;
	}

	String GetFileName()
	{
		return mFileName;
	}

	inline int GetMeshCount()
	{
		return mMeshes.Size();
	}

	inline int GetCameraCount()
	{
		return mCameras.Size();
	}

	inline void SetRoot(Node * pRoot)
	{
		mRoot = pRoot;
	}

	inline Node * GetRoot()
	{
		return mRoot;
	}

	inline void Update()
	{
		mRoot->Update();
	}

	inline void Apply()
	{
		mRoot->Apply();
	}

	void AddClip(AnimationClip * pClip, AnimTrackEnum track, unsigned int numOfLoops, float speed )
	{
		AnimationClipLogic * pClipLogic = new AnimationClipLogic(pClip, numOfLoops, speed);		
		mModelAnimations.AddClip(pClipLogic, track);
	}

	inline ModelAnimations & GetModelAnimations()
	{
		return mModelAnimations;
	}

	void SetLayer(int layer);

private:
	ModelAnimations mModelAnimations;
	Node * mRoot;
	String mFileName;
	bool mAnimating;
};


#endif //__ModelData__