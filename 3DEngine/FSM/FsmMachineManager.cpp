#include "FsmMachineManager.h"

NEWDEL_IMPL(FsmMachineManager)
FsmMachineManager::FsmMachineManager()
{
	
}

void FsmMachineManager::AddMachine(FsmStateMachine * pMachine)
{
	mStateMachines.Add(pMachine);
}

void FsmMachineManager::RemoveMachine(FsmStateMachine * pMachine)
{
	mStateMachines.Remove(pMachine);
}

void FsmMachineManager::OnTick()
{

	ListIterator listIter(mStateMachines);
	while (FsmStateMachine * pMachine = (FsmStateMachine*)listIter.GetPtr())
	{
		if ( pMachine->IsEnabled() )
			pMachine->OnTick();
	}

}