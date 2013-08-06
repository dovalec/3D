
#ifndef __SoundManager__
#define __SoundManager__

#include "../Config.h"
#include "../TL/MyString.h"

#define MAX_SOUND_ARRAY 128

//#include "../../../me2013/assets/content/SoundEnums.h"

class SoundManager {
public:
	SoundManager();
public:
	NEWDEL_DECL

	inline static SoundManager * GetSoundManager()
	{
		return GlobalHeap::GetSoundManager();	
	}

	void SetSoundState(String type, bool state);
    
	inline const String * GetSoundArray()
	{
		return mSoundArray;
	}
	inline void ResetSoundArray()
	{
		for (int n=0;n<MAX_SOUND_ARRAY;n++)
        {
            mSoundArray[n] = "";
        }
	}

protected:
	String mSoundArray[MAX_SOUND_ARRAY];
};


#endif //__SoundManager__