#ifndef __BotLauncherData__
#define __BotLauncherData__

#include "Config.h"
#include "Game/BotLauncher.h"

class BotLauncherData
{
public:
	NEWDEL_DECL
	BotLauncherData(void);
	virtual ~BotLauncherData(void);

	void AllocateBotLaunchers(int numofBotLaunchers);

	inline int GetNumOfBotLaunchers()
	{
		return mNumOfBotLaunchers;
	}

	inline BotLauncher * GetBotLauncher(int index)
	{
		return &mBotLaunchers[index];
	}

	inline BotLauncher * GetBotLauncher(String name)
	{
		int index = (int)mBotLaunchersTable.GetByKey(name.GetPtr());
		return &mBotLaunchers[index];
	}

	BotLauncher * mBotLaunchers;
	int mNumOfBotLaunchers;
	Map mBotLaunchersTable;
};


#endif //__BotLauncherData__