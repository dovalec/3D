#include "Door.h"

NEWDEL_IMPL(Door)
NEWDEL_IMPL(DoorEdge)



Door::Door(void)
{
	mState = door_state_close;
}

Door::~Door()
{
	delete [] mDoorEdges;
}

void Door::AllocateDoorEdges(int numOfDoorsEdges)
{
	mDoorEdges = new DoorEdge[numOfDoorsEdges];
	mNumOfDoorEdges = numOfDoorsEdges;
}

