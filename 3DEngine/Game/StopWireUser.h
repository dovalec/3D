#ifndef __StopWireUser__
#define __StopWireUser__

#define MAX_STOP_WIRES 256

class StopWireUser {
public:
	NEWDEL_DECL;
	StopWireUser()
	{
		mNumStopWires = 0;
	}

	void AddStopWire(Wire * pWire)
	{
		mStopWires[mNumStopWires] = pWire;
		mNumStopWires++;
		if (mNumStopWires == MAX_STOP_WIRES)
		{
			LOGE("mNumStopWires == MAX_STOP_WIRES");
			STOP;
		}
	}

	void RemoveStopWire(Wire * pWire)
	{
		for (int n=0;n<mNumStopWires;n++)
		{
			if (mStopWires[n] == pWire)
			{
				mNumStopWires--;
				mStopWires[n] = mStopWires[mNumStopWires];
				break;
			}
		}
	}

protected:
	int mNumStopWires;
	Wire * mStopWires[MAX_STOP_WIRES];

};


#endif //__StopWireUser__