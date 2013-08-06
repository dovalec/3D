#ifndef __Door__
#define __Door__

#include "Config.h"

class DoorEdge
{
public:
	NEWDEL_DECL

	float x1;
	float z1;
	float x2;
	float z2;

	float nx;
	float ny;
	float nz;

};


class Door
{
public:
	NEWDEL_DECL
	Door(void);
	virtual ~Door(void);

	enum DoorState {
		door_state_open = 0,
		door_state_close,
	};

	inline int GetNumOfDoorEdges()
	{
		return mNumOfDoorEdges;
	}

	inline DoorEdge * GetDoorEdge(int index)
	{
		return &mDoorEdges[index];
	}

	void AllocateDoorEdges(int numOfDoorEdges);

	inline DoorState GetState()
	{
		return mState;
	}

	inline void SetState(DoorState state)
	{
		mState = state;
	}

protected:
	DoorState mState;

private:
	int mNumOfDoorEdges;
	DoorEdge * mDoorEdges;
};

#endif //__Vector3__

