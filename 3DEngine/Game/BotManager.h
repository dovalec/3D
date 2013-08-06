#ifndef __BotManager__
#define __BotManager__

#include "../Config.h"
#include "../TL/List.h"
#include "BotNode.h"

class BotLauncher;

class BotManager {

public:
	NEWDEL_DECL
	BotManager();
	virtual ~BotManager();

	void AddBotNode(BotNode * pBotNode);
	void RemoveBotNode(BotNode * pBotNode);
	
	/*
	inline BotNode * GetBotNode(int index)
	{
		if (index >= mNumBotNodes)
		{
			LOGE("index >= mNumBotNodes");
			STOP;
		}

		return mBotNodes[index];
	}
	*/
	
	//void SetBotLauncher(BotLauncher * pBotLauncher)
	//{
	//	mBotLauncher = pBotLauncher;
	//}

	//inline BotLauncher * GetBotLauncher()
	//{
	//return mBotLauncher;
	//}

	inline int GetNumBotNodes()
	{
		return mBotNodes.Size();
	}

	inline List & GetBotNodes()
	{
		return mBotNodes;
	}

	inline void OnTick()
	{
		ListIterator botNodesIter(mBotNodes);
		while (BotNode * pBotNode = (BotNode*)botNodesIter.GetPtr())
		{
			pBotNode->OnTick();
		}
		
	}


protected:
	List mBotNodes;
	BotLauncher * mBotLauncher;
	
};

#endif //__BotManager__
