#ifndef __FsmMachineManager__
#define __FsmMachineManager__

#include "../Config.h"
#include "../TL/List.h"
#include "FsmStateMachine.h"

#define MAX_NUM_FSM_MACHINES 256
class FsmMachineManager {
public:
	FsmMachineManager();
public:
	NEWDEL_DECL
	inline static FsmMachineManager & GetFsmMachineManager()
	{
		return *GlobalHeap::GetFsmMachineManager();
	}

	void Destroy()
	{
		ListIterator listIter(mStateMachines);
		while (FsmStateMachine * pMachine = (FsmStateMachine*)listIter.GetPtr())
		{
			delete pMachine;
		}
	}

	void AddMachine(FsmStateMachine * pMachine);
	void RemoveMachine(FsmStateMachine * pMachine);
	void OnTick();

protected:
	List mStateMachines;
};

#endif //__FsmMachineManager__
