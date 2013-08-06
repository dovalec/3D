#ifndef __BotFsmMachine__
#define __BotFsmMachine__

#include "../Config.h"
#include "../FSM/FSMStateMachine.h"
#include "../ModelData.h"
#include "BotFsmState.h"

#include "ChasePlayerNode.h"

class BotNode;
class BotManager;

class BotFsmMachine : public FsmStateMachine
{
public:
	NEWDEL_DECL
	BotFsmMachine();
	virtual ~BotFsmMachine();

	void SetBotNode(BotNode * pBotNode);
	inline BotNode * GetBotNode()
	{
		return mBotNode;	
	}

protected:
	BotNode * mBotNode;
};

#endif //__BotFsmMachine__