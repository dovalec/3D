#ifndef __AnimationManager__
#define __AnimationManager__

#include "../Config.h"
#include "../Loader.h"
#include "../TL/Map.h"

#include "AnimationClip.h"
#include "AnimationUVClip.h"

class AnimationManager
{
public:
	AnimationManager(void);
	~AnimationManager(void);

public:
	NEWDEL_DECL
	inline static AnimationManager & GetAnimationManager()
	{
		return *GlobalHeap::GetAnimationManager();
	}


	AnimationClip * LoadClip(const char * clipName);
	AnimationUVClip * LoadUVClip(const char * clipName);
	bool ReadClip(AnimationClip * pClip, Loader & loader);
	bool ReadUVClip(AnimationUVClip * pClip, Loader & loader);
	void Destroy();
	void DestroyClip(const char * clipName);

private:
	Map mClipMap;
	
};


#endif // __AnimationManager__