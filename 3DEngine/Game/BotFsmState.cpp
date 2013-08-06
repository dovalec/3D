
#include "BotFsmState.h"
#include "BotFsmMachine.h"

NEWDEL_IMPL(BotFsmState)
BotFsmState::BotFsmState()
{
	mMachine = NULL;
}

BotFsmState::~BotFsmState()
{

}
