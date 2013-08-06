
#include "FsmState.h"
#include "FsmStateMachine.h"

NEWDEL_IMPL(FsmState)
FsmState::FsmState()
{
	mMachine = NULL;
}

FsmState::~FsmState()
{

}

void FsmState::setMachine(FsmStateMachine * pMachine)
{
	mMachine = pMachine;
}