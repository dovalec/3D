#include "DoorsData.h"
#include "ModelData.h"

NEWDEL_IMPL(DoorsData)


DoorsData::DoorsData(void)
{
	
}

DoorsData::~DoorsData(void)
{
	delete [] mDoors;
}

void DoorsData::AllocateDoors(int numOfDoors)
{
	mDoors = new Door[numOfDoors];
	mNumOfDoors = numOfDoors;
}
