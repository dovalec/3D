
#include "FsmStateMachine.h"
#include "FsmMachineManager.h"

NEWDEL_IMPL(FsmStateMachine)
FsmStateMachine::FsmStateMachine()
{
	mCurrentState = NULL;
	mNewState = NULL;
	mDoSwitchState = false;
	mEnabled = true;
	mAddToManager = true;

	FsmMachineManager & fsmMachineManager = FsmMachineManager::GetFsmMachineManager();
	fsmMachineManager.AddMachine(this);

}

FsmStateMachine::FsmStateMachine(bool addToManager)
{
	mCurrentState = NULL;
	mNewState = NULL;
	mDoSwitchState = false;
	mEnabled = true;
	mAddToManager = addToManager;

	if (mAddToManager)
	{
		FsmMachineManager & fsmMachineManager = FsmMachineManager::GetFsmMachineManager();
		fsmMachineManager.AddMachine(this);
	}
}


FsmStateMachine::~FsmStateMachine()
{
	if (mAddToManager)
	{
		FsmMachineManager & fsmMachineManager = FsmMachineManager::GetFsmMachineManager();
		fsmMachineManager.RemoveMachine(this);
	}
}

void FsmStateMachine::OnTick()
{
	
	if (mDoSwitchState)
	{
		mDoSwitchState = false;
		if (mCurrentState)
			mCurrentState->OnLeave();

		mCurrentState = mNewState;
		mCurrentState->OnEnter();
	}
	else
	{
		mCurrentState->OnTick();
	}
}

void FsmStateMachine::SwitchToState(FsmState * state)
{
	if (mCurrentState == state)
		return;

	mNewState = state;
	mDoSwitchState = true;
}


void FsmStateMachine::AddState(FsmState * pState)
{
	pState->setMachine(this);
}
