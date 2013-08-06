#include "BotLauncherData.h"

NEWDEL_IMPL(BotLauncherData)
BotLauncherData::BotLauncherData(void)
{
	
}

BotLauncherData::~BotLauncherData(void)
{
	delete [] mBotLaunchers;
}


void BotLauncherData::AllocateBotLaunchers(int numofBotLaunchers)
{
	mBotLaunchers = new BotLauncher[numofBotLaunchers];
	mNumOfBotLaunchers = numofBotLaunchers;
}
