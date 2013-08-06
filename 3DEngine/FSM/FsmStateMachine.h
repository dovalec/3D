#ifndef __FsmStateMachine__
#define __FsmStateMachine__

#include "../Config.h"
#include "FsmState.h"


class FsmStateMachine
{
public:
	NEWDEL_DECL
	FsmStateMachine();
	FsmStateMachine(bool addToManager);
	virtual ~FsmStateMachine();

	virtual void OnTick();

	void SwitchToState(FsmState * state);
	inline FsmState * GetCurrentState()
	{
		return mCurrentState;
	}

	inline bool IsEnabled()
	{
		return mEnabled;
	}

	void Enable()
	{
		mEnabled = true;
	}

	void Disable()
	{
		mEnabled = false;
	}


	void AddState(FsmState * pState);
	
protected:
	FsmState * mCurrentState;
	FsmState * mNewState;
	bool mDoSwitchState;
	bool mEnabled;
	bool mAddToManager;


};

#endif //__FsmStateMachine__