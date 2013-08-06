#ifndef __BotFsmState__
#define __BotFsmState__

#include "../Config.h"
#include "../FSM/FSMState.h"

class BotFsmMachine;

class BotFsmState : public FsmState
{
public:
	NEWDEL_DECL
	BotFsmState();
	virtual ~BotFsmState();
};

#endif //__BotFsmState__