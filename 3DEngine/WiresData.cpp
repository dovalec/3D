#include "WiresData.h"
#include "ModelData.h"

NEWDEL_IMPL(WiresData)
NEWDEL_IMPL(WirePoint)
NEWDEL_IMPL(Wire)


WiresData::WiresData(void)
{
	
}

WiresData::~WiresData(void)
{
	delete [] mWires;
}

void WiresData::AllocateWires(int numOfWires)
{
	mWires = new Wire[numOfWires];
	mNumOfWires = numOfWires;
}

