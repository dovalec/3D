#include "LocatorData.h"

NEWDEL_IMPL(LocatorData)
NEWDEL_IMPL(Locator)

LocatorData::LocatorData(void)
{
	
}

LocatorData::~LocatorData(void)
{
}


void LocatorData::AllocateLocators(int numofLocators)
{
	mLocators = new Locator[numofLocators];
	mNumOfLocators = numofLocators;
}
