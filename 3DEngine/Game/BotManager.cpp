#include "BotManager.h"
#include "BotLauncher.h"

NEWDEL_IMPL(BotManager)
BotManager::BotManager()
{

}

BotManager::~BotManager()
{
	ListIterator botNodesIter(mBotNodes);
	while (BotNode * pBotNode = (BotNode*)botNodesIter.GetPtr())
	{
		delete pBotNode;
	}	
}

void BotManager::AddBotNode(BotNode * pBotNode)
{
	mBotNodes.Add(pBotNode);
	pBotNode->SetBotManager(this);	
}

void BotManager::RemoveBotNode(BotNode * pBotNode)
{
	mBotNodes.Remove(pBotNode);
}

