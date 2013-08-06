#ifndef __AnimationClip__
#define __AnimationClip__

#include "../Config.h"
#include "../TL/MyString.h"
#include "AnimationKeys.h"

enum AnimTrackEnum
{
	ANIM_TRACK_0 = 0, 
	ANIM_TRACK_1,
	ANIM_TRACK_2,
	ANIM_TRACK_3,
	ANIM_TRACK_4,
	ANIM_TRACK_5,
	ANIM_TRACK_6,
	ANIM_TRACK_7,
	ANIM_TRACK_LAST,
};

class Node;

struct TransformAndKeys
{
	NEWDEL_DECL
	int transformIndex;
	AnimationKeys keys;
};

class AnimationClip
{
public:
	NEWDEL_DECL

	enum ClipType {
		clip_transform,
		clip_uv
	};

	AnimationClip(void);
	virtual ~AnimationClip(void);

	void AllocateTransformKeys(int numOfTransforms);
	TransformAndKeys & GetTransformAndKeysByNum(int num);

	inline int GetNumOfTransforms()
	{
		return mNumOfTransforms;
	}

	inline float GetDuration()
	{
		return mDuration;
	}

	inline void SetDuration(float duration)
	{
		mDuration = duration;
	}

	inline void SetNumKeys(int numKeys)
	{
		if (mNumKeys != 0 && mNumKeys!=numKeys)
		{
			LOGE("mNumKeys != 0 && mNumKeys!=numKeys");
			assert(0);
		}
		mNumKeys = numKeys;
	}

	inline unsigned int GetNumKeys()
	{
		return mNumKeys;
	}

	inline ClipType GetClipType()
	{
		return mClipType;
	}

public:
	String mName;
protected:
	float mDuration;
	int mNumKeys;
	ClipType mClipType;

private:
	int mNumOfTransforms;
	TransformAndKeys * mpTransformKeys;

};


#endif // __AnimationClip__