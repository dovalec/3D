#ifndef __FsmState__
#define __FsmState__

#include "../Config.h"

class FsmStateMachine;

class FsmState
{
public:
	NEWDEL_DECL
	FsmState();
	virtual ~FsmState();
	void setMachine(FsmStateMachine * pMachine);

	virtual void OnTick()= 0;
	virtual void OnEnter() = 0;
	virtual void OnLeave() = 0;
	
protected:
	FsmStateMachine * mMachine;
};

#endif //__FsmState__