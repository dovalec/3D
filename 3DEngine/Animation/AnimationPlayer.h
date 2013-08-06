#ifndef __AnimationPlayer__
#define __AnimationPlayer__

#include "../Config.h"
#include "../Loader.h"
#include "../ModelData.h"
#include "../TL/List.h"

#include "AnimationClip.h"


/*
#define ANIM_TRACK_4 4
#define ANIM_TRACK_5 5
#define ANIM_TRACK_6 6
#define ANIM_TRACK_7 7
*/
class AnimationPlayer
{
public:
	NEWDEL_DECL
	AnimationPlayer(void);
	~AnimationPlayer(void);

public:
	inline static AnimationPlayer & GetAnimationPlayer()
	{
		return *GlobalHeap::GetAnimationPlayer();
	}

	void Destroy();

	
	bool PlayClip(ModelData * pModelData, const char * clipName, AnimTrackEnum track,  unsigned int loopCount = 1, float speed = 1.0f);
	bool PlayUVClip(ModelData * pModelData, const char * clipName, AnimTrackEnum track,  unsigned int loopCount = 1, float speed = 1.0f);
	
	int RemoveCurrentClip(ModelData * pModelData, AnimTrackEnum track);
	void ClearTrack(ModelData * pModelData, AnimTrackEnum track);

	inline 	bool IsPlaying(ModelData * pModelData, AnimTrackEnum track)
	{
		return !pModelData->GetModelAnimations().IsEmpty(track);
	}

	void RemoveAllAnimatedModelsDebug( AnimTrackEnum track);
	void OnTick();

private:
	List mAnimatedModels;
};


#endif // __AnimationPlayer__