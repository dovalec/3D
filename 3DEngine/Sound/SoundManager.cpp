#include "SoundManager.h"

NEWDEL_IMPL(SoundManager)
SoundManager::SoundManager()
{
	ResetSoundArray();
}

void SoundManager::SetSoundState(String type, bool state)
{
	//mSoundArray[type] = (int)state;
}