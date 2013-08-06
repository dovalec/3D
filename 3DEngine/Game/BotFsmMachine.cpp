
#include "BotFsmMachine.h"

#include "BotNode.h"
#include "BotManager.h"

NEWDEL_IMPL(BotFsmMachine)
BotFsmMachine::BotFsmMachine()
{

}

BotFsmMachine::~BotFsmMachine()
{

}

void BotFsmMachine::SetBotNode(BotNode * pBotNode)
{
	mBotNode = pBotNode;	
}
